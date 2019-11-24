# Agent Evolution and Trajectory Analysis

This project incoorporates a simple genetic algorithm technique in order to evaluate the outcomes of different payoff matrices.

The goal is to evaluate trajectories that are only influenced by genome mutation, matrix mutation or both together. Phenotypes are calculated through a simple matrix * genome multiplication.

For optimal execution, all parameters are passed before compilation so agents can be generated on the stack.

## Compilation

### makefile

```bash
make
```

Generates an executable file named `bin/agentevolver`. Delete `agentevolver` and every file in `bin/objs` using `make clean`.

The default values for the simulation can be viewed in [defines.hpp](src/defines.hpp). Values can be passed to `make` in order to overwrite those values.

The following parameters are available:

```bash
POPULATION   # Number of agents per generation
GENSIZE      # Dimension of genome
PHENSIZE     # Dimension of phenotype
PAYOFF       # Payoff matrix
AGENTSTART   # Genome vector that every agent starts with
GENOME_PROB  # Probability for genome index to mutate (1 = 100%)
MATRIX_PROB  # Probability for matrix index to mutate
GENERATIONS  # Amount of generations
```

If each value is specified, calling `make` may look something like this:

```bash
make \
    POPULATION=1000 \
    GENSIZE=3 \
    PHENSIZE=3 \
    PAYOFF="{{0,1,-1},{-1,0,1},{1,-1,0}}" \
    AGENTSTART="{0.333,0.333,0.333}" \
    GENOME_PROB=0.0001 \
    MATRIX_PROB=0 \
    GENERATIONS=10000
```

In this instance, each generation has a population of 1000. The payoff matrix resembles a classic rock-paper-scissors game where each agent starts off with a mixed strategy that makes each move is equally likely. For each agent reproducing, there is a 0.01% chance for an index in the genome vector to mutate, whereas the matrix stays constant.

### outcreator.py

This python script was implemented to generate executables for multiple payoff matrices. Together with `outexec.py`, whose job is to run those simulations multiple times, they should form a convenient way of handling those executables.

View [outcreator.py](outcreator.py) for more details on setup.

### manually

Files can also be compiled manually in the source folder:

```bash
g++ -c -std=c++17 -o main.o main.cpp
g++ -c -std=c++17 -o agent.o agent.cpp
g++ -c -std=c++17 -o matrix.o matrix.cpp
g++ -c -std=c++17 -o matrixexception.o matrixexception.cpp
g++ -c -std=c++17 -o randwrap.o randwrap.cpp
g++ -c -std=c++17 -o world.o world.cpp
g++ -c -std=c++17 -o fileagent.o fileagent.cpp
g++ -o agentevolver.out -std=c++17 main.o agent.o matrix.o matrixexception.o randwrap.o world.o fileagent.o
```

To change the simulation values, add each parameter listed above as an argument to each compilation step as follows: `-D{parameter}={value}` (do not include curly braces).

### increasing stack size

For a large population size you may get a segmentation fault. Using `g++`, you can increase the stack size by adding `-Wl,-stack_size,{value}` as a parameter. `make` has this `value` set to `0x100000000` - on my machine this is enough to host over 10,000,000 agents.

## Execution

Once the file is compiled, the only argument that can be passed to the executable is the filename of where to save the resulting csv-file. If no filename is specified, it will be saved in `result/csvs/` with a name corresponding to its payoff matrix (setting + and - for positive and negative payoffs). If the file already exists, a number will be added.

[outexec.py](outexec.py) can also be used to run a simulation multiple times. Simply calling it causes all .out files inside the `bin` directory to be executed. Each resulting .csv is then saved in its appropriate directory inside `results/csvs`. Additionally multithreading should help speed up the process - a parameter inside the python script can be changed to adjust the number of threads generated.

## Plotting

Inside `results/` call [drawings.py](results/drawings.py) to convert all csvs into plots. Simulations with 3-dimensional genomes will also be plottet into ternary files.
