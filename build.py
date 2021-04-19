import subprocess
import sys



if (subprocess.run([sys.executable,"D:/boot/src/main.py","5","compile","src","build","arduino:avr:uno"]).returncode!=0):
	sys.exit(1)
if ("--run" in sys.argv):
	if (subprocess.run([sys.executable,"D:/boot/src/main.py","5","upload","build","COM3","arduino:avr:uno"]).returncode!=0):
		sys.exit(1)
