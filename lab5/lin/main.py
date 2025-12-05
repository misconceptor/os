import subprocess
import sys

print("input numbers:")
input_data = sys.stdin.readline().strip()# + "\n"

print("[p]", end=' ')
p = subprocess.Popen(['python3', 'p.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
p_out, _ = p.communicate(input=input_data)
print(p_out.strip())

print("[a]", end=' ')
a = subprocess.Popen(['python3', 'a.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
a_out, _ = a.communicate(input=p_out)
print(a_out.strip())

print("[m]", end=' ')
m = subprocess.Popen(['python3', 'm.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
m_out, _ = m.communicate(input=a_out)
print(m_out.strip())

print("[s]", end=' ')
s = subprocess.Popen(['python3', 's.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
s_out, _ = s.communicate(input=m_out)
print(s_out.strip())
