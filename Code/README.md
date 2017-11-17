# PipelinedMIPS
Simulator for a pipelined processor that draws upon Tomasulo’s algorithm and scoreboarding.

Built for Computer Architecture - CDA 5155: Fall 2017 at the University of Florida

Compiling and running the code:

g++ *.cpp -o MIPSsim

./MIPSsim sample.txt

This will generate a file called simulation.txt which will have the cycle by cycle simulation of the processor operations.
A sample simulation.txt which is the output when the file fed in is sample.txt is included in the folder.

More details of the instructions executed can be found in the Instructions specifications pdf in the Documents folder.
For an overview -
It executes instructions of 5 types

1)BRANCH

2)NOP

3)BREAK

4)Regular ALU

5)Mem Type ALU

More details of the project can be found in the project specification pdf in the Document folder.

I see potential bugs in the usage of RegisterController class for setting the Registers to busy and then making 
them free again for avoiding various kinds of hazards. I will work on them when I have a bit more time.

Suggestions for improving the codebase are most welcome.