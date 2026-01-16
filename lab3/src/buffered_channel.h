#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <utility>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : buffer_size(size > 0 ? size : 1), closed(false) {}

    ~BufferedChannel() {
        Close();
    }

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw std::runtime_error("Cannot send to closed channel");
        }

        cv_space.wait(lock, [this]() { 
            return queue.size() < buffer_size || closed; 
        });

        if (closed) {
            throw std::runtime_error("Cannot send to closed channel");
        }

        queue.push(std::move(value));
        cv_data.notify_one();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mtx);

        cv_data.wait(lock, [this]() { 
            return !queue.empty() || closed; 
        });

        if (queue.empty()) {
            return std::make_pair(T(), false);
        }

        T value = std::move(queue.front());
        queue.pop();

        cv_space.notify_one(); 
        return std::make_pair(std::move(value), true); 
    }

    void Close() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!closed) {
            closed = true; 
            cv_space.notify_all();
            cv_data.notify_all();
        }
    }

private:
    std::queue<T> queue;
    const size_t buffer_size;
    bool closed;

    std::mutex mtx;
    std::condition_variable cv_space; 
    std::condition_variable cv_data;  
};

#endif