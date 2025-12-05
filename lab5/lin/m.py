import sys
a = list(map(int, sys.stdin.read().split()))
for i in range(len(a)):
    print(a[i]*7, end=' ')
print()
