import subprocess
import threading
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
import termcolor
import os

folder = 'results/csvs'
runs = 40

exec_total = 0
exec_count = 0

threadLock = threading.Lock()

def filename_to_nested_folder(name):
    return os.path.join(folder, *name.split('_')).replace('.out', '')

def call_subprocess(executable: str):
    global runs
    global exec_total
    global exec_count
    
    result_folder = filename_to_nested_folder(executable)
    filename = os.path.join(result_folder, executable.replace('.out', ''))

    f = open('{}_log.txt'.format(filename), "w")

    args = ["./bin/{}".format(executable), '-f', filename, '-r', str(runs)]
    print(subprocess.list2cmdline(args))
    subprocess.run(args, stdout=f)
    
    with threadLock:
        exec_count += 1
        print(termcolor.colored('{} / {} ({:.2f}%) finished'.format(exec_count, exec_total, float(exec_count * 100) / exec_total), 'green', attrs=['bold']) + ' - finished {}'.format(executable))

if __name__ == "__main__":
    def isExec(filename):
        return filename.endswith(".out")
    
    executables = list(filter(isExec, os.listdir("bin")))
    for file in executables:
        dir = filename_to_nested_folder(file)
        # dir = '{}/{}'.format(folder, file.replace('.out', ''))
        if not os.path.exists(dir):
            os.makedirs(dir)

    exec_total = len(executables)
    runs_total = exec_total * runs
    pool = ThreadPool(8)
    print(executables)
    pool.map(call_subprocess, executables)