import subprocess
from datetime import datetime

def simulateGames(
    agents=1000,
    opponents=100,
    generations=1000,
    mutationProbs=(0.01,0.01,0.01),
    filename=None
):
    mutProbsStr = ','.join([str(x) for x in mutationProbs])
    if filename is None:
        filename = '{} -a={}-o={}-g={}.csv'.format(datetime.now().strftime('%Y-%m-%d %H:%M'), agents, opponents, generations)

    command = '../bin/agentevolver -a {} -o {} -g {} -p {} -f "{}" -s , -v'.format(agents, opponents, generations, mutProbsStr, filename)
    print(command)


simulateGames()