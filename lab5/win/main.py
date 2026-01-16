import subprocess
import sys
def run_pipeline():
    pM = subprocess.Popen([sys.executable, 'm.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    pA = subprocess.Popen([sys.executable, 'a.py'], stdin=pM.stdout, stdout=subprocess.PIPE, text=True)
    pP = subprocess.Popen([sys.executable, 'p.py'], stdin=pA.stdout, stdout=subprocess.PIPE, text=True)
    pS = subprocess.Popen([sys.executable, 's.py'], stdin=pP.stdout, stdout=subprocess.PIPE, text=True)

    pM.stdout.close()
    pA.stdout.close()
    pP.stdout.close()

    print("Numbers separated by space: ")
    data = sys.stdin.readline()
    
    pM.stdin.write(data + "\n")
    pM.stdin.close()

    result = pS.communicate()[0]
    print(f"Answer: {result.strip()}")

if __name__ == "__main__":
    run_pipeline()