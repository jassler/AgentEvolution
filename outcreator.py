#!usr/bin/python3

import subprocess
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
    "{{0,-1,-1},{-1,0,-1},{-1,-1,0}}",

    "{{0,1},{1,0}}",
    "{{0,1},{-1,0}}",
    "{{0,-1},{1,0}}",
    "{{0,-1},{-1,0}}"
]

if __name__ == "__main__":
    for payoff in payoffs:
        phensize = payoff.count('},{') + 1
        gensize = phensize
        agentstart = '{{{}}}'.format(','.join([str(1.0 / gensize)] * gensize))

        arglist = ["make",
                "POPULATION={}".format(100000),
                "PAYOFF=\"{}\"".format(payoff),
                "AGENTSTART=\"{}\"".format(agentstart),
                "GENSIZE={}".format(gensize),
                "PHENSIZE={}".format(phensize)
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
                .replace(";", ",")
        os.rename("bin/agentevolver", "bin/{}.out".format(filename))
