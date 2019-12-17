import subprocess
import threading
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
import termcolor
import os

folder = 'results/csvs'
runs = 100

exec_total = 0
exec_count = 0

threadLock = threading.Lock()

def call_subprocess(executable: str):
    global runs
    global exec_total
    global exec_count
    
    result_folder = os.path.join(folder, executable.replace('.out', ''))
    filename = os.path.join(result_folder, executable.replace('.out', ''))

    args = ["./bin/{}".format(executable), '-f', filename, '-r', str(runs), '>>', folder]
    print(subprocess.list2cmdline(args))
    subprocess.run(args)
    
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
    pool = ThreadPool(4)
    print(executables)
    pool.map(call_subprocess, executables)
    
    print('done')