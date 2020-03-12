#!usr/bin/python3

import subprocess
import argparse
import itertools
import shutil
import os
import threading
import termcolor
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool


# use flag -i for convenient input
# ... later
# parser = argparse.ArgumentParser()
# parser.add_argument('-i', '--interactive')
# args = parser.parse_args()
# if args.interactive:
#     print('in development')
#     exit()

generations = 1024

payoffs = [
    # "{{0,2,-1},{-1,0,2},{2,-1,0}}",
    # "{{0,3,-1},{1,0,1},{3,-1,0}}",
    # "{{0,1,1},{-1,0,3},{1,1,0}}",
    # "{{0,6,-4},{-3,0,5},{-1,3,0}}",
    # "{{0,1,1},{1,0,1},{1,1,0}}",
    # "{{0,3,-1},{3,0,-1},{1,1,0}}",
    # "{{0,1,3},{-1,0,5},{1,3,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    # "{{0,-1,3},{-1,0,3},{1,1,0}}",
    # "{{0,1,1},{-1,0,1},{-1,-1,0}}",
    # "{{0,1,1},{1,0,1},{-1,-1,0}}",
    # "{{0,1,-1},{1,0,1},{1,-1,0}}",
    # "{{0,-1,-1},{1,0,1},{-1,1,0}}",
    # "{{0,-1,1},{-1,0,1},{-1,-1,0}}",
    # "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    # "{{0,-1,-1},{1,0,-3},{-1,-3,0}}",
    # "{{0,1,-1},{-3,0,1},{-1,1,0}}",
    # "{{0,1,-3},{1,0,-1},{-3,-1,0}}",
    # "{{0,-3,-1},{-3,0,-1},{-1,-1,0}}"
    # "{{0,-1,1},{1,0,-1},{-1,1,0}}",
    # "{{0.05,-1,1},{1,0.05,-1},{-1,1,0.05}}",
    # "{{0.1,-1,1},{1,0.1,-1},{-1,1,0.1}}",
    # "{{0.2,-1,1},{1,0.2,-1},{-1,1,0.2}}",
    # "{{0.3,-1,1},{1,0.3,-1},{-1,1,0.3}}"

    # "{{0,1,-1},{-1,0,1},{1,-1,0}}",
    # "{{0,1,-1},{1,0,1},{1,-1,0}}",
    # "{{0,1,1},{-1,0,1},{1,1,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    # "{{0,1,1},{1,0,1},{1,1,0}}",
    # "{{0,1,1},{-1,0,1},{1,1,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    # "{{0,-1,1},{-1,0,1},{1,1,0}}"
    # "{{0,1,1},{-1,0,1},{-1,-1,0}}",
    # "{{0,1,1},{1,0,1},{-1,-1,0}}",
    # "{{0,1,-1},{1,0,1},{1,-1,0}}",
    # "{{0,-1,-1},{1,0,1},{-1,1,0}}",
    # "{{0,-1,1},{-1,0,1},{-1,-1,0}}",
    # "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    # "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    # "{{0,1,-1},{1,0,-1},{-1,-1,0}}",
    # "{{0,-1,-1},{-1,0,-1},{-1,-1,0}}"
    # "{{0,1},{1,0}}",
    # "{{0,1},{-1,0}}",
    # "{{0,-1},{1,0}}",
    # "{{0,-1},{-1,0}}",
    # "{{1,1},{2,0}}",
    # "{{2,1},{3,0}}",
    # "{{1,0},{0,0}}",
    # "{{4,0},{0,0}}"
    "{{1,0},{0,1}}",
    "{{100,99},{99,100}}"
]

agents = [
    "{{1,0}},{{0,1}}"
]

genome_probs = [
    0
]

matrix_probs = [
    0
]

populations = [
    1024,
    1000000
]


total = 1
done = 0

threadLock = threading.Lock()

# 0 = arglist for compilation
# 1 = object directory string
def compile(arglist):
    global threadLock
    global total
    global done

    with open(os.devnull, 'w') as devnull:
        subprocess.run(arglist[0], stdout=devnull)
    shutil.rmtree(arglist[1])

    with threadLock:
        done += 1
        print(termcolor.colored('{} / {} ({:.2f}%)'.format(done, total, float(done * 100) / total), ['red', 'yellow', 'green'][int((done-1) * 3 / total)], attrs=['bold']), end='\r')

if __name__ == "__main__":
    f = open("bin/compilations.txt", "w")

    calls_to_make = []

    for payoff, agentstart, genome_prob, matrix_prob, population in itertools.product(payoffs, agents, genome_probs, matrix_probs, populations):
        if genome_prob == 0 and matrix_prob == 0 and agentstart.count('},{') == 0:
            continue

        phensize = payoff.count('},{') + 1
        gensize = agentstart.split('}')[0].count(',') + 1

        filename = payoff \
                .replace("-1", "-") \
                .replace("1", "+") \
                .replace("},{", ";") \
                .replace("{", "") \
                .replace("}", "") \
                .replace(",", "") \
                .replace(";", ",") + \
            agentstart \
                .replace('{{', '_') \
                .replace('}}', '_') \
                .replace('},{', '-') \
                .replace('{', '_') \
                .replace('}', '_') + \
            'g{},m{},pop{}'.format(genome_prob, matrix_prob, population)
        
        odir = 'bin'
        objdir = os.path.join(odir, 'objs_{}'.format(filename))
        ofile = os.path.join(odir, '{}.out'.format(filename))

        arglist = ["make",
                "POPULATION={}".format(population),
                'PAYOFF ="{}"'.format(payoff),
                'AGENTSTART ="{}"'.format(agentstart),
                "GENSIZE={}".format(gensize),
                "PHENSIZE={}".format(phensize),
                "GENOME_PROB={}".format(genome_prob),
                "MATRIX_PROB={}".format(matrix_prob),
                "GENERATIONS={}".format(generations),

                "LOG_DEBUG={}".format(1),
                "LOG_PROGRESS={}".format(0),

                "ODIR={}".format(odir),
                "OBJDIR={}".format(objdir),
                "OFILE={}".format(ofile)
        ]
        
        print(termcolor.colored(subprocess.list2cmdline(arglist), 'green', attrs=['bold', 'underline']))

        f.write(subprocess.list2cmdline(arglist) + '\n')
        calls_to_make.append([arglist, objdir])

    f.close()

    total = len(calls_to_make)
    pool = ThreadPool(8)
    pool.map(compile, calls_to_make)

    
