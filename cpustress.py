# Import the module
import subprocess
from time import sleep
import csv


# Ask the user for input
increment = raw_input("Define Load Increment: ")

startingValue = raw_input("At what value should the stressing begin?: ")

cpuCore = raw_input("Which core on the machine would you like to stress?: ")

# Set up the echo command and direct the output to a pipe
#p1 = subprocess.Popen(['taskset', '-c', cpuCore, "./stress-ng", '--cpu', '1', '--cpu-load', increment, '--cpu-method', 'loop', '--timeout', '20'], stdout=subprocess.PIPE)

f = open("data/consoleoutput.txt", "w")

for x in range(0,20):
        #Prepare loads x time file with header
        with open('data/loadsxtimeatpriority' +str(x) +'.csv', mode='w') as csvfile:
                csvfile = csv.writer(csvfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
                csvfile.writerow(['Load', 'Avg. NS', 'Avg. Ticks'])
        for i in range(int(startingValue), 101, int(increment)):
                command1 = ['taskset', '-c', cpuCore, "./stress-ng", '--cpu', '1', '--cpu-load', str(i), '--cpu-method', 'loop', '--timeout', '10']
                commandString1 = ' '.join(command1)

                print(commandString1)

                command2 = ['taskset', '-c', cpuCore, './a.out', str(i), str(x)]
                commandString2 = ' '.join(command2)


                print(commandString2)


                commands = [command1, command2]


                ps = []
                for singlecommand in commands:
                        p = subprocess.Popen(singlecommand,stdout = f)
                        sleep(7)
                        ps.append(p)

                for p in ps:
                        p.wait()
        
