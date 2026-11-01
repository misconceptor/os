#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using vd = std::vector<double>;

template<typename Func>
inline double measure_time(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ans = end - start;
    return ans.count();
}

inline double l2_norm(const vd &a) {
    double ans = 0.0;
    for (auto &i : a)
        ans += i * i;
    return sqrt(ans);
}

inline double residue(const vd &a, const vd &b) {
    double ans = 0.0;
    int n = (int)a.size();
    for (int i = 0; i < n; ++i) 
        ans += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(ans);
}

inline void print_head(const vd &a, int n = 5) {
    for (int i = 0; i < std::min(n, (int)a.size()); ++i)
        std::cout << a[i] << ' ';
    std::cout << std::endl;
}