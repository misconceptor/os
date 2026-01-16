import subprocess
import sys

def run_linux_pipeline():
    pM = subprocess.Popen(['./m.py'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    pA = subprocess.Popen(['./a.py'], stdin=pM.stdout, stdout=subprocess.PIPE, text=True)
    pP = subprocess.Popen(['./p.py'], stdin=pA.stdout, stdout=subprocess.PIPE, text=True)
    pS = subprocess.Popen(['./s.py'], stdin=pP.stdout, stdout=subprocess.PIPE, text=True)

    pM.stdout.close()
    pA.stdout.close()
    pP.stdout.close()

    nums = input("Enter numbers: ")
    pM.stdin.write(nums + "\n")
    pM.stdin.close()

    print(f"Answer: {pS.communicate()[0].strip()}")

if __name__ == "__main__":
    run_linux_pipeline()