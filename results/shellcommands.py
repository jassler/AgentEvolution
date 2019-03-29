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
        filename = 'csvs/{} -a={}-o={}-g={}.csv'.format(datetime.now().strftime('%Y-%m-%d %H:%M'), agents, opponents, generations)

    # command = '../bin/agentevolver -a {} -o {} -g {} -p {} -f "{}" -s , -v'.format(agents, opponents, generations, mutProbsStr, filename)
    command = [
        '../bin/agentevolver',
        '-a', str(agents),
        '-o', str(opponents),
        '-g', str(generations),
        '-p', mutProbsStr,
        '-f', filename,
        '-s', ';',
        '--outstart', '',
        '--outend', '\n',
        '-v'
    ]
    
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)

    for line in iter(process.stdout.readline, b''):
        print(line)
    
    process.wait()



simulateGames()