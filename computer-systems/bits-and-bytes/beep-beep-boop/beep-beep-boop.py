import sys
import tty

tty.setcbreak(0)

# does not beep on integrate vscode terminal

while True:
    ch = sys.stdin.read(1)
    if ch.isdigit():
        for x in range(0, int(ch)):
            sys.stdout.buffer.write(b"\x07")
            sys.stdout.flush
