# generate data for mooc

from random import *
import os, sys, math

data_file_name = "input.txt"

ncmds = 100000
MAXP = (1e6)*2 + 1
def gen_file(filename):
	with open(filename, 'w') as f:
		f.write(str(ncmds) + '\n')
		# for i in range(ncmds):
		# 	f.write('I ' + str(randint(1, MAXP)) + '\n')
		for i in range(ncmds):
			r = random()
			pts = ''
			if r < 0.8:
				t = 'I'
				pts = str(randint(1, MAXP))
			elif r > 0.8 and r < 0.90:
				t = 'H'
			elif r > 0.90 and r < 1.00:
				t = 'L'
			else:
				i -= 1
				continue
			f.write(t + ' ' + pts + '\n')

def main():
	gen_file(data_file_name)


if __name__ == "__main__":
	main()