import sys
import os
import subprocess

wd = os.getcwd()
ex_dir = sys.argv[1]
os.chdir(ex_dir)
os.system('gcc *.c')

for test_input in filter(lambda x: x[-3:] == '.in', os.listdir()):
    print(test_input + '======================')
    os.system(f'a.exe < {test_input} > actual.out')
    os.system(f'diff actual.out {test_input[:-2] + "out"}')
    print('\n')

os.chdir(wd)
