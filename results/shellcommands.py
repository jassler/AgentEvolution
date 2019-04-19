import subprocess, os, sys
from datetime import datetime

import pandas as pd

def update_progress(progress, in_IPython: bool = True):
    bar_length = 40
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
    if progress < 0:
        progress = 0
    if progress >= 1:
        progress = 1
    block = int(round(bar_length * progress))
    text = '\rProgress: [{0}] {1:.1f}%'.format( '#' * block + '-' * (bar_length - block), progress * 100)
    print('{}'.format(text), end='')

def simulate_generations(args: dict = {}, print_progress: bool = True):
    arguments = {
        '--agents':            '1000',
        '--opponents':         '16',
        '--generations':       '10000',
        '--winners':           '500',
        '--genomestart':       '{}',
        '--matrixstart':       '1,0,0/0,1,0/0,0,1',
        '--probabilities':     '0.001,0.001,0.001',
        '--matrixmutation':    '0.001',
        '--payoff':            '0,-1,1/1,0,-1/-1,1,0',
        '--file':              None,
        '--separator':         ',',
        '--percentmultiplier': '1000',
        '--infofile':          '1',
        '--verbose':           '1'
    }
    
    for key, value in args.items():
        arguments[key] = value
    
    arguments['--outformat'] = '%p%n'
    
    # generate file name. Counts up if file already exists
    if arguments['--file'] is None:
        filename = '/Users/felixfritz/Desktop/AgentEvolution/results/csvs/{},{},{} ({})'.format(
            arguments['--agents'],
            arguments['--opponents'],
            arguments['--generations'],
            arguments['--payoff'].replace('/', ')('))
        
        tmp_filename = filename
        counter = 0
        while os.path.isfile(tmp_filename + '.csv'):
            counter += 1
            tmp_filename = filename + ' ' + str(counter)
        
        arguments['--file'] = tmp_filename + '.csv'
    
    # create command to be executed
    command = ['../bin/agentevolver']
    for key, value in arguments.items():
        if key != '--verbose' and key != '--infofile':
            command.extend((key, value))
        elif value == '1' or value.lower() == 'true' or value.lower() == 'yes':
            command.append(key)
    
    command_str = subprocess.list2cmdline(command)
    
    # process = subprocess.Popen(command_str, shell=True, stdout=subprocess.PIPE)
    process = subprocess.Popen(command_str, shell=True, stdout=subprocess.PIPE)
    had_newline = True
    for line in iter(process.stdout.readline, b''):
        l = line.decode('utf-8').replace('\n', '')
        if l.replace('.', '').isdigit() and print_progress:
            p = float(l)
            update_progress(p / 100.0, False)
            had_newline = False
        else:
            if not had_newline:
                print()

            print(l)
            had_newline = True
    
    process.wait()

    return arguments['--file']

if __name__ == '__main__':
    prev = ''
    arguments = {}

    files = []

    # arguments to pass to evolver program
    for arg in sys.argv:
        if prev.startswith('--'):
            arguments[prev] = arg
        prev = arg
    
    # run simulation n times
    for i in range(1, 6):
        print('\n>>> running simulation #{} <<<'.format(i))
        files.append(simulate_generations(arguments))
        arguments['--infofile'] = '0'
    
    # LOD avg
    print('\ncalculating LOD averages...')
    headers = [ 'LOD Rock Genome', 'LOD Paper Genome', 'LOD Scissor Genome', 'LOD Rock Phenotype', 'LOD Paper Phenotype', 'LOD Scissor Phenotype' ]
    df = pd.read_csv(files[0], usecols=headers)

    for file in files[1:]:
        df2 = pd.read_csv(file, usecols=headers)
        df = pd.concat([df, df2], axis=1, keys=['x', 'y'])
        df = df.swaplevel(0, 1, axis=1).groupby(level=0, axis=1).mean()
    
    df.index.name = 'Generation'
    df.to_csv('{} avg.csv'.format(files[0].replace('.csv', '')))
    print('done')