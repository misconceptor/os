#ifndef BUFFERED_CHANNEL_H
#define BUFFERED_CHANNEL_H

#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <deque>
#include <utility>

template <typename T>
class buffered_channel {
public:
    explicit buffered_channel(size_t buffer_size) :
        capacity_(buffer_size), closed_(false) {
        if (buffer_size == 0) {
            throw std::invalid_argument("size=0");
        }
    }

    buffered_channel(const buffered_channel&) = delete;
    buffered_channel& operator=(const buffered_channel&) = delete;
    buffered_channel(buffered_channel&&) = delete;
    buffered_channel& operator=(buffered_channel&&) = delete;

    void send(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this] {
            return queue_.size() < capacity_ || closed_;
        });
        if (closed_) {
            throw std::runtime_error("Attempt to send data through a closed channel.");
        }
        queue_.push_back(std::move(value));
        lock.unlock();
        not_empty_.notify_one();
    }
    std::pair<T, bool> recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] {
            return !queue_.empty() || closed_;
        });
        if (queue_.empty() && closed_) {
            return {T(), false};
        }
        T value = std::move(queue_.front());
        queue_.pop_front();
        
        lock.unlock();
        not_full_.notify_one();

        return {std::move(value), true};
    }

    void close() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (closed_) return;

        closed_ = true;
        lock.unlock();
        
        not_empty_.notify_all();
        not_full_.notify_all();
    }

private:
    std::deque<T> queue_;
    const size_t capacity_;
    bool closed_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
};

#endif // BUFFERED_CHANNEL_H