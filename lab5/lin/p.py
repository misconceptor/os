import sys
a = list(map(int, sys.stdin.read().split()))
for i in range(len(a)):
    print(a[i]*a[i]*a[i], end=' ')
print()
