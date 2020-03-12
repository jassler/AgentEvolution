import drawings
import numpy
from argparse import ArgumentParser
import os

def normalize(arr, divby=None):
    if divby is None:
        divby = sum(arr)
    if divby == 0:
        print('dividing by 0')
        return arr
    return [x / divby for x in arr]

def calc_replicatordynamic(distribution: list, payoffs):
    epsilon = 0.000001
    if sum(distribution) < 1 - epsilon or sum(distribution) > 1 + epsilon:
        print('normalizing', distribution, '(sum =', sum(distribution), ')')
        distribution = normalize(distribution)
    
    fitnesses = []
    for i in range(len(distribution)):
        fitness = 0
        for j in range(len(distribution)):
            fitness += distribution[j] * payoffs[i][j]
        fitnesses.append(fitness)
    
    all_fitness = 0
    for i in range(len(distribution)):
        all_fitness += distribution[i] * fitnesses[i]
    
    replicators = []
    for i in range(len(distribution)):
        replicators.append(distribution[i] * (fitnesses[i] - all_fitness))
    return replicators


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument("-f", "--file", dest="filename",
                        help="File to plot", metavar="FILE")
    parser.add_argument("-p", "--payoff", dest="payoff", help="Payoff matrix (2d-array, eg [[1,0,0],[0,1,0],[0,0,1]])")
    parser.add_argument("-s", "--start", dest="start", help="Starting condition (eg [0.5,0.25,0.25])")
    parser.add_argument("-d", "--divisor", dest="div", help="What to divide by (higher is more accurate)")
    parser.add_argument("-e", "--epsilon", dest="epsilon", type=float, help="Epsilon for RPS")
    args = parser.parse_args()

    distr = normalize([1.0 / 3.0 + 0.0001, 1.0 / 3.0, 1.0 / 3.0])
    e = 0
    if args.epsilon is not None:
        e = args.epsilon
    p = [
        [e, -1, 1],
        [1, e, -1],
        [-1, 1, e]
    ]


    if args.start is not None:
        distr = eval(args.start)
    if args.payoff is not None:
        p = eval(args.payoff)
    div = 10
    if args.div is not None:
        div = int(args.div)

    p_str = [','.join(str(x) for x in y) for y in p]
    filename = 'eulers/{} - {} - {}'.format('_'.join(p_str), ','.join(str(round(x, 5)) for x in distr), div)
    
    if args.filename is not None:
        filename = args.filename
    else:
        count = 0
        ext = '.png'
        while os.path.exists('{}{}'.format(filename, ext)):
            count += 1
            ext = ' {}.png'.format(count)
        filename = '{}{}'.format(filename, ext)


    print(distr)
    print(p)
    print(div)
    print(filename)
    points = [distr]

    distr = normalize(distr)
    while len(points) < 1000000 and not any(x > 0.99999 for x in distr):
        dir = calc_replicatordynamic(distr, p)
        new_distr = [distr[x] + dir[x] / div for x in range(len(distr))]
        new_distr = normalize(new_distr)
        points.append(new_distr)
        distr = new_distr
    print('plotting')
    drawings.ternary_plot(points, save_file=filename, line_color_start='black', labels=('R','S','P'))#, line_color_end='#107bb7')
    print('done')
