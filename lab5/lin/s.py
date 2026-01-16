#!/usr/bin/env python3
import sys
a = list(map(int, sys.stdin.read().split()))
ans=0
for i in range(len(a)):
    ans=ans+a[i]
print(ans)
