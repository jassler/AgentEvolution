import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from multiprocessing.dummy import Pool as ThreadPool
import os

plt.switch_backend('Agg')

def ternary_plot(data,
                 title = '',
                 labels = ('R', 'P', 'S'),
                 sides = 3,
                 start_angle = 90,
                 fig_args = {'figsize':(18,18),'facecolor':'white','edgecolor':'white'},
                 edge_args = {'color':'black','linewidth':2},
                 line_color = (0, 0, 0),
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
    end = len(newdata[:,0]) - 1
    # for i in range(0,end):
    #     ax.plot(newdata[:,0][i:i+2], newdata[:,1][i:i+2], '.-', linewidth=2, alpha=0.99, c=(0, i / end, i / end))
    ax.plot(newdata[:,0], newdata[:,1], linewidth=2, alpha=0.99, c='#603f40')
    
    # fig.show()
    
    if save_file is not None:
        fig.savefig(save_file)
    plt.close(fig)

def draw_file(filename: str, outfile: str = None):
    if outfile is None:
        outfile = 'pngs/{}.png'.format(filename.replace('.csv', ''))
    
    headers = []
    colors=['grey', 'green', 'orange', 'cyan', 'red', 'black']
    df = pd.read_csv(filename, sep=';')

    for h in df.head():
        if h.startswith('phenotype_'):
            headers.append(h)
    
    fig = plt.figure()
    ax = fig.gca()
    
    for i in range(0, len(headers)):
        ax.plot(df['generation'], df[headers[i]], color=colors[i%len(colors)], linewidth=2, label=headers[i])

    ax.set_xlabel('Generation')
    ax.set_ylabel('Probability')
    ax.set_title(filename.replace('.csv', ''))
    ax.legend()
    fig.savefig(outfile)
    plt.close(fig)


def plot_file(filename: str):
    headers = []
    df = pd.read_csv(os.path.join(plot_file.csv_folder, filename), sep=';')

    for h in df.head():
        if h.startswith('phenotype_'):
            headers.append(h)
    
    draw_file(os.path.join(csv_folder, filename), os.path.join(plot_file.png_folder, filename.replace('.csv', '.png')))

    if len(headers) != 3:
        return

    ternary_plot(df[headers],
            title=os.path.basename(filename).replace('.csv', ''),
            labels=tuple(x.replace('_', ' ') for x in headers),
            save_file=os.path.join(plot_file.ternary_folder, os.path.basename(filename).replace('.csv', '.png')))


if __name__ == '__main__':
    amount = len(os.listdir('csvs'))
    count = 0
    for folder in os.listdir('csvs'):
        ternary_folder = os.path.join('ternary', folder)
        png_folder = os.path.join('pngs', folder)
        csv_folder = os.path.join('csvs', folder)

        if not os.path.exists(png_folder):
            os.makedirs(png_folder)

        if not os.path.exists(ternary_folder):
            os.makedirs(ternary_folder)
        
        plot_file.ternary_folder = ternary_folder
        plot_file.csv_folder = csv_folder
        plot_file.png_folder = png_folder
        pool = ThreadPool(4)
        pool.map(plot_file, os.listdir(csv_folder))
        
        count += 1
        print('{} / {}'.format(count, amount), end='\r')
