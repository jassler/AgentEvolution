import itertools
import copy

dimensions = 3

def prepare_all_matrices(dimensions: int):
    matrices = []
    m = [1] * (dimensions ** 2 - dimensions)

    while sum(m) != -len(m):
        matrices.append([
            [0, m[0], m[1]],
            [m[2], 0, m[3]],
            [m[4], m[5], 0]
        ])

        # is this binary?
        i = 0
        while m[i] < 0:
            m[i] *= -1
            i += 1
        
        m[i] *= -1

    matrices.append([
        [0, m[0], m[1]],
        [m[2], 0, m[3]],
        [m[4], m[5], 0]
    ])

    return matrices

def swap_column(matrix, col1, col2):
    for item in matrix:
        item[col1], item[col2] = item[col2], item[col1]

def swap_row(matrix, row1, row2):
    matrix[row1], matrix[row2] = matrix[row2], matrix[row1]

def transpose(matrix):
    for y in range(len(matrix)):
        for x in range(y, len(matrix[0])):
            matrix[x][y], matrix[y][x] = matrix[y][x], matrix[x][y]

def print_matrix(m):
    print('[')
    for row in m:
        print('  [{}]'.format(', '.join(['{:2d}'.format(x) for x in row])))
    print(']')

matrices = prepare_all_matrices(dimensions)
stable_classes = []

for matrix in matrices:
    is_stable = True

    for swapper in itertools.combinations(range(dimensions), 2):

        m = copy.deepcopy(matrix)
        swap_column(m, swapper[0], swapper[1])
        swap_row(m, swapper[0], swapper[1])

        if m in stable_classes:
            is_stable = False
            break
    
    if is_stable:
        # make sure transposed matrix also doesn't appear
        m = copy.deepcopy(matrix)
        transpose(m)
        if m not in stable_classes:
            stable_classes.append(matrix)

for i, m in enumerate(stable_classes):
    print('{}: '.format(i), end='')
    print_matrix(m)

print('total number:', len(stable_classes))