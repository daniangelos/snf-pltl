#!/usr/bin/env python3
import os
import sys

liverpool = "/home/dani/Work/mestrado/liverpool/"
path = liverpool + "snf-pltl/"
benchmarks = liverpool + "benchmarks/"
target = path + "pltl"
ex = path + "ex"
valgrind = "valgrind --leak-check=yes" 

def run(inputfile):
    outputfile = inputfile + ".output" 
    systemout = inputfile + ".systemout"
    print("Processing " + inputfile)
    time = "timeout 300s /usr/bin/time -f \"%e\" "
    command = time + valgrind + " " + target + " <" + inputfile + " >" + outputfile + " 2>" + systemout
    os.system(command)
    #sys.exit()

def getfiles(path):
    for entry in os.listdir(path):
        if(os.path.isdir(path + entry)): 
            getfiles(path + entry + "/")
        else:
            if(entry.endswith(".tspass") or entry.endswith(".trp")):
                run(path + entry)

def clear(path):
    for entry in os.listdir(path):
        if(os.path.isdir(path + entry)): 
            os.system("rm -f " + path + entry + "/*.output")
            clear(path + entry + "/")

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == "rm":
        clear(benchmarks)

    getfiles(benchmarks)
