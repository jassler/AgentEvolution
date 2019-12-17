#!usr/bin/python3

import subprocess
import itertools
import os
import termcolor

payoffs = [
    "{{0,1,-1},{-1,0,1},{1,-1,0}}",
    "{{0,1,-1},{1,0,1},{1,-1,0}}",
    "{{0,1,1},{-1,0,1},{1,1,0}}",
    "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    "{{0,1,1},{1,0,1},{1,1,0}}",
    "{{0,1,1},{-1,0,1},{1,1,0}}",
    "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    "{{0,-1,1},{-1,0,1},{1,1,0}}",
    "{{0,1,1},{-1,0,1},{-1,-1,0}}",
    "{{0,1,1},{1,0,1},{-1,-1,0}}",
    "{{0,1,-1},{1,0,1},{1,-1,0}}",
    "{{0,-1,-1},{1,0,1},{-1,1,0}}",
    "{{0,-1,1},{-1,0,1},{-1,-1,0}}",
    "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    "{{0,-1,-1},{1,0,-1},{-1,-1,0}}",
    "{{0,1,-1},{-1,0,1},{-1,1,0}}",
    "{{0,1,-1},{1,0,-1},{-1,-1,0}}",
    "{{0,-1,-1},{-1,0,-1},{-1,-1,0}}"
]

agents = [
    r'{0.3333333333333333,0.3333333333333333,0.3333333333333333}',
    r'{0.25,0.25,0.5}',
    r'{0.3,0.2,0.5}',
    r'{0.35,0.15,0.5}',
    r'{0.4,0.1,0.5}',
    r'{0.2,0.3,0.5}',
    r'{0.15,0.35,0.5}',
    r'{0.1,0.4,0.5}',
]

if __name__ == "__main__":
    for payoff, agentstart in itertools.product(payoffs, agents):
        phensize = payoff.count('},{') + 1
        gensize = agentstart.count(',') + 1
        agentstart = '{' + agentstart + '}'

        # POPULATION=1000
        # GENSIZE=3
        # PHENSIZE=3
        # PAYOFF="{{0,1,-1},{-1,0,1},{1,-1,0}}"
        # AGENTSTART="{0.333,0.333,0.333}"
        # GENERATIONS=10000
        # GENOME_PROB=0.0001
        # MATRIX_PROB=0

        # LOG_DEBUG=1
        # LOG_PROGRESS=0
        # LOG_GENERATION=1

        arglist = ["make",
                "POPULATION={}".format(10000),
                "PAYOFF=\"{}\"".format(payoff),
                "AGENTSTART=\"{}\"".format(agentstart),
                "GENSIZE={}".format(gensize),
                "PHENSIZE={}".format(phensize),
                "GENOME_PROB={}".format(0.0001),
                "MATRIX_PROB={}".format(0),
                "GENERATIONS={}".format(2500),

                "LOG_DEBUG={}".format(0),
                "LOG_PROGRESS={}".format(0)
        ]

        print(termcolor.colored(subprocess.list2cmdline(arglist), 'green', attrs=['bold', 'underline']))
        
        subprocess.run(["make", "clean"])
        subprocess.run(arglist)

        filename = payoff \
                .replace("-1", "-") \
                .replace("1", "+") \
                .replace("},{", ";") \
                .replace("{", "") \
                .replace("}", "") \
                .replace(",", "") \
                .replace(";", ",") + agentstart.replace('{', '-').replace('}', '')
        os.rename("bin/agentevolver", "bin/{}.out".format(filename))
