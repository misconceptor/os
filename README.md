# single thread
## 1. 
    taskset -c 0 ./helper

    baseline
    loops
    baseline_block 2000 50
    baseline_block_avx 2000 50
## 2.
    baseline block N = 2000, B = [list]
## 3.
    baseline block AVX N = 2000, B = [list]

# multi thread

