import subprocess
import threading
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
import termcolor
import os

folder = 'results/csvs'
runs = 50

exec_total = 0
runs_total = 0

exec_count = 0
runs_count = 0

threadLock = threading.Lock()

def call_subprocess(executable: str):
    global runs
    global runs_total
    global runs_count
    global exec_total
    global exec_count
    
    filename_placeholder = '{0}/{1}/{1}_{{}}.csv'.format(folder, executable.replace('.out', ''))

    count = 0
    for _ in range(0,runs):
        while os.path.exists(filename_placeholder.format(count)):
            count += 1
        
        filename = filename_placeholder.format(count)
        subprocess.run(["./bin/{}".format(executable), filename])

        with threadLock:
            runs_count += 1
        print('{:.2f}% runs'.format(float(runs_count * 100) / runs_total))
    
    with threadLock:
        exec_count += 1
        print(termcolor.colored('{} / {} ({:.2f}%) finished'.format(exec_count, exec_total, float(exec_count * 100) / exec_total), 'green', attrs=['bold']) + ' - finished {}'.format(executable))

if __name__ == "__main__":
    def isExec(filename):
        return filename.endswith(".out")
    
    executables = list(filter(isExec, os.listdir("bin")))
    for file in executables:
        dir = '{}/{}'.format(folder, file.replace('.out', ''))
        if not os.path.exists(dir):
            os.makedirs(dir)

    exec_total = len(executables)
    runs_total = exec_total * runs
    pool = ThreadPool(2)
    pool.map(call_subprocess, executables)
    
    print('done')