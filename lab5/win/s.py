import sys
ans = 0
for line in sys.stdin:
    if not line.strip(): break
    for x in line.split():
        val = int(x)
        ans += val
print(ans)