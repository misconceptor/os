import sys
import subprocess
def run_pipeline(programs, input_line):
    input_bytes = (input_line + '\n').encode('ascii')
    current_input = input_bytes
    for i, prog in enumerate(programs):
        result = subprocess.run([prog], input=current_input, 
                               capture_output=True, text=False)
        if result.returncode != 0:
            print(f"Error in {prog}: {result.stderr.decode()}")
            return
        current_input = result.stdout
        print(f"{prog}: {result.stdout.decode().strip()}")
    return result.stdout.decode().strip()

if __name__ == "__main__":
    input_line = sys.stdin.readline().strip()
    output = run_pipeline(["a.exe", "m.exe", "p.exe", "s.exe"], input_line)
    print(f"answer: {output}")