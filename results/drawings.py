import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from multiprocessing.dummy import Pool as ThreadPool
from argparse import ArgumentParser
import shutil
import threading
from os import path
import os

import colour

plt.switch_backend('Agg')

def ternary_plot(data,
                 title = '',
                 labels = ('R', 'P', 'S'),
                 sides = 3,
                 start_angle = 90,
                 fig_args = {'figsize':(18,18),'facecolor':'white','edgecolor':'white'},
                 edge_args = {'color':'black','linewidth':4},
                 line_color_start='black',
                 line_color_end = None,
                 rotate_labels = False,
                 label_offset = 0.10,
                 scaling = False,
                 show = True,
                 save_file = None
):
    basis = np.array([
        [
            np.cos(2*x*np.pi/sides + start_angle*np.pi/180),
            np.sin(2*x*np.pi/sides + start_angle*np.pi/180)
        ] for x in range(sides)
    ])

    basis_str = 'np.array([[' + '],['.join('\n\tnp.cos(2*{}*np.pi/{} + {}*np.pi/180,\n\tnp.sin(2*{}*np.pi/{} + {}*np.pi/180'.format(x, sides, start_angle, x, sides, start_angle) for x in range(sides)) +']])'
    print('\n\n')
    print(basis_str)
    print('\n\n')
    print(basis)

    if scaling:# If data is Nxsides, newdata is Nx2.
        newdata = np.dot(data.div(data.sum(axis=1), axis=0),basis)# Scales data for you.
    else:
        newdata = np.dot(data,basis)# Assumes data already sums to 1.

    if not show:
        return newdata
        
    fig = plt.figure(**fig_args)
    ax = fig.add_subplot(111)
    ax.text(-0.5, 0.3, title, rotation=60, horizontalalignment='center', verticalalignment='center', size=34)
    
    for i,l in enumerate(labels):
        if i >= sides:
            break
        
        x = basis[i,0]
        y = basis[i,1]
        if rotate_labels:
            angle = 180 * np.arctan(y/x)/np.pi + 90
            if angle > 90 and angle <= 270:
                angle = np.mod(angle + 180, 360)
        else:
            angle = 0
        
        ax.text(
            x * (1 + label_offset),
            y * (1 + label_offset),
            l,
            horizontalalignment='center',
            verticalalignment='center',
            rotation=angle,
            size=34
        )
        
    # clear normal axes
    ax.set_xticks(())
    ax.set_yticks(())
    ax.set_frame_on(False)
    
    # plot border
    ax.plot(
        [ basis[x,0] for x in list(range(sides)) + [0,] ],
        [ basis[x,1] for x in list(range(sides)) + [0,] ],
        **edge_args
    )
    
    # extra room for labels
    ax.set_xlim(left=-1.2, right=1.2)
    ax.set_ylim(bottom=-1.2, top=1.2)
    
    # plot
    if line_color_end is None:
        ax.plot(newdata[:,0], newdata[:,1], linewidth=4, alpha=0.99, c=line_color_start)
    else:
        # color gradient
        end = len(newdata[:, 0]) - 1
        colors = colour.Color(line_color_start).range_to(line_color_end, end)
        for i in range(0, end):
            ax.plot(newdata[:,0][i:i+2], newdata[:,1][i:i+2], '.-', linewidth=5, c=next(colors).rgb)
    
    # fig.show()
    
    if save_file is not None:
        fig.savefig(save_file, transparent=True, bbox_inches='tight')
    plt.close(fig)

def draw_file(filename: str, outfile: str, headers, title: str = '', labels=None):
    colors=['grey', 'green', 'orange', 'cyan', 'red', 'black']
    df = pd.read_csv(filename, sep=';', nrows=201)
    
    fig = plt.figure()
    ax = fig.gca()

    if labels is None:
        labels = ['x_{}'.format(x+1) for x in range(len(headers))]
    
    for i in range(0, len(headers)):
        ax.plot(df[headers[i]], color=colors[i%len(colors)], linewidth=2, label=labels[i])

    ax.set_xlabel('Generation')
    ax.set_ylabel('Probability')
    ax.set_title(title)
    ax.legend()
    ax.grid()
    fig.savefig(outfile)
    plt.close(fig)

threadLock = threading.Lock()
total = 1
progress = 0


def plot_file(filename: str, dest_plotline: str = None, dest_ternary: str = None, header_start: str = 'phenotype_', title=None):
    '''
    plot_file takes csv file path as input and plots to corresponding destinations (png format)
    if dest_xxx is None, no plot is produced

    - dest_plotline: line plot to output. Set to None if it should be ignored
    - dest_ternary: ternary plot, only applies if headers have a length of 3 and
      name is not None
    - header_start: headers to look for in csv. We assume they come in a format of "name_1", "name_2", "name_3", ...
    '''
    global total
    global progress

    headers = []
    df = pd.read_csv(filename, sep=';')

    for h in df.head():
        if h.startswith(header_start):
            headers.append(h)
    
    if title is None:
        title = path.split(filename)[-1].replace('.png', '')
    
    # draw 2d plot
    if dest_plotline is not None:
        draw_file(filename, dest_plotline, headers, title=title)

    # draw ternary plot
    if len(headers) == 3 and dest_ternary is not None:
        ternary_plot(df[headers],
                title="",#os.path.basename(filename).replace('.csv', ''),
                # labels=tuple(x.replace('LOD_phenotype_0', 'Rock').replace('LOD_phenotype_1', 'Scissors').replace('LOD_phenotype_2', 'Paper') for x in headers),
                labels=('0', '1', '2'),
                save_file=dest_ternary,
                line_color_start='#107bb7')
    
    # show progress
    with threadLock:
        progress += 1
    print('{} / {} done ({}%)'.format(progress, total, int(progress * 100 / total)), end='\r')

def prepare_files(src: str, dest: list) -> dict:
    '''
    prepare_files copies folder structure from src to dest
    returns dictionary:
    - key: path to csv file in src
    - value: path to png file without src (dest not prepended, must be done manually)
    '''
    csv_files = {}

    # create "root" directories
    for d in dest:
        if not path.exists(d):
            os.mkdir(d)

    for root, dirs, files in os.walk(src, topdown=True):
        root_without_src = root.replace(src, '').strip('/')

        # copy folder structure
        for dir in dirs:
            p = path.join(root_without_src, dir)
            for d in dest:
                final = path.join(d, p)
                if not path.exists(final):
                    os.mkdir(final)
        
        # search csvs
        for f in files:
            if f.endswith('avg.csv'):
                csv_files[path.join(root, f)] = path.join(root_without_src, f.replace('.csv', '.png'))
    
    return csv_files

if __name__ == '__main__':

    parser = ArgumentParser()
    parser.add_argument("-f", "--file", dest="filename",
                        help="File to plot", metavar="FILE")
    parser.add_argument("-d", "--destfolder", dest="destfolder", type=str, help="Destination folder to put pngs and ternary")
    parser.add_argument("-r", "--readfolder", dest="readfolder", type=str, help="(nested) csv folder to read from")
    parser.add_argument("-t", "--title", dest="title", type=str, help="Title for plot")
    args = parser.parse_args()

    if args.filename is not None:
        
        destinations = [os.path.splitext(args.filename)[0] + '_lod.png', os.path.splitext(args.filename)[0] + '_lod_ternary.png']
        plot_file(args.filename, destinations[0], destinations[1], header_start='LOD_phenotype_', title=args.title)
        destinations = [os.path.splitext(args.filename)[0] + '_pop.png', os.path.splitext(args.filename)[0] + '_pop_ternary.png']
        plot_file(args.filename, destinations[0], destinations[1], title=args.title)
        
        print("Plottet", args.filename, "to", destinations[0], "and", destinations[1])
        exit()
    
    print('Preparing folder structure')

    readfolder = 'csvs'
    writefolder = ''
    if args.readfolder is not None:
        readfolder = args.readfolder
    if args.destfolder is not None:
        writefolder = args.destfolder

    csv_files = prepare_files(readfolder, ['{}/ternary'.format(writefolder), '{}/pngs'.format(writefolder)])
    total = len(csv_files) * 2
    for key, value in csv_files.items():    
        plot_file(key, dest_plotline=path.join('{}/pngs'.format(writefolder), value), dest_ternary=path.join('{}/ternary'.format(writefolder), value), title=args.title)
        plot_file(key, dest_plotline=path.join('{}/pngs'.format(writefolder), value.replace('.png', '_LOD.png')), dest_ternary=path.join('{}/ternary'.format(writefolder), value.replace('.png', '_LOD.png')), header_start='LOD_phenotype_', title=args.title)
    
    print('\ndone')
    # pool = ThreadPool(4)

    # print('Plotting...')
    # pool.map(plot_file, csv_files)

