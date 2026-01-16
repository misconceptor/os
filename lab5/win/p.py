import sys
for line in sys.stdin:
    if not line.strip(): break
    for x in line.split():
        print(int(x) ** 3)
    sys.stdout.flush()