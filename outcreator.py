#!usr/bin/python3

import subprocess
import itertools
import shutil
import os
import threading
import termcolor
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool

payoffs = [
    "{{0,1,-1},{-1,0,1},{1,-1,0}}",
    # "{{0,1,-1},{1,0,1},{1,-1,0}}",
    # "{{0,1,1},{-1,0,1},{1,1,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    # "{{0,1,1},{1,0,1},{1,1,0}}",
    # "{{0,1,1},{-1,0,1},{1,1,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}"
    # "{{0,-1,1},{-1,0,1},{1,1,0}}",
    "{{0,1,1},{-1,0,1},{-1,-1,0}}",
    "{{0,1,1},{1,0,1},{-1,-1,0}}",
    # "{{0,1,-1},{1,0,1},{1,-1,0}}",
    # "{{0,-1,-1},{1,0,1},{-1,1,0}}",
    # "{{0,-1,1},{-1,0,1},{-1,-1,0}}",
    # "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    # "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    # "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    "{{0,1,-1},{1,0,-1},{-1,-1,0}}",
    "{{0,-1,-1},{-1,0,-1},{-1,-1,0}}"
    # "{{0,1},{1,0}}",
    # "{{0,1},{-1,0}}",
    # "{{0,-1},{1,0}}",
    # "{{0,-1},{-1,0}}"
]

# agents = [
#     r'{1,0}',
#     r'{0,1}',
#     r'{0.5,0.5}',
#     r'{0.25,0.75}',
#     r'{0.75,0.25}'
# ]
agents = [
    r'{0.333,0.333,0.333}',
    r'{0.25,0.25,0.5}',
    r'{0.3,0.2,0.5}',
    r'{0.75,0.15,0.1}',
#     r'{0.4,0.1,0.5}',
#     r'{0.2,0.3,0.5}',
#     r'{0.15,0.35,0.5}',
#     r'{0.1,0.4,0.5}',
    r'{1,0,0}',
    r'{0,1,0}',
    r'{0,0,1}'
]

genome_probs = [
    0,
    0.0001,
    0.01,
    0.25
]

matrix_probs = [
    0,
    0.01
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

    for payoff, agentstart, genome_prob, matrix_prob in itertools.product(payoffs, agents, genome_probs, matrix_probs):
        if genome_prob == 0 and matrix_prob == 0:
            continue

        phensize = payoff.count('},{') + 1
        gensize = agentstart.count(',') + 1
        agentstart = '{' + agentstart + '}'

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
                .replace('}}', '_') + \
            '{},{}'.format(genome_prob, matrix_prob)
        
        odir = 'bin'
        objdir = os.path.join(odir, 'objs_{}'.format(filename))
        ofile = os.path.join(odir, '{}.out'.format(filename))

        arglist = ["make",
                "POPULATION={}".format(1024),
                "PAYOFF=\"{}\"".format(payoff),
                "AGENTSTART=\"{}\"".format(agentstart),
                "GENSIZE={}".format(gensize),
                "PHENSIZE={}".format(phensize),
                "GENOME_PROB={}".format(genome_prob),
                "MATRIX_PROB={}".format(matrix_prob),
                "GENERATIONS={}".format(10000),

                "LOG_DEBUG={}".format(1),
                "LOG_PROGRESS={}".format(0),

                'ODIR="{}"'.format(odir),
                'OBJDIR="{}"'.format(objdir),
                'OFILE="{}"'.format(ofile)
        ]
        
        print(termcolor.colored(subprocess.list2cmdline(arglist), 'green', attrs=['bold', 'underline']))
        f.write(subprocess.list2cmdline(arglist) + '\n')
        calls_to_make.append([arglist, objdir])

    f.close()

    total = len(calls_to_make)
    pool = ThreadPool(8)
    pool.map(compile, calls_to_make)

    
