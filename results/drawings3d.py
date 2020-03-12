import matplotlib as mpl
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt
import math

def get_cartesian_from_barycentric(b, t):
    return t.dot(b)

# baricentrig coordinates
mpl.rcParams['legend.fontsize'] = 10

fig = plt.figure()
ax = fig.gca(projection='3d')

vertices = [(0, 0, 0), (1, 0, 0), (0.5, 0.5*math.sqrt(3), 0), (0.5, 1.0/6.0*math.sqrt(3), 1.0/3.0*math.sqrt(6))]
conn = [vertices[x] for x in [0, 1, 2, 0, 3, 1, 2, 3]]

ax.plot([c[0] for c in conn], [c[1] for c in conn], [c[2] for c in conn], label='parametric curve')

vertices = np.transpose(np.array([[0, 0, 0], [1, 0, 0], [0.5, 0.5*math.sqrt(3), 0], [0.5, 1.0/6.0*math.sqrt(3), 1.0/3.0*math.sqrt(6)]]))


ax.legend()

df = pd.read_csv('/Users/felixfritz/Documents/GitHub/AgentEvolution/results/csvs/0+00,00+0,000+,+000/0,0,0,1/0,0.001/0+00,00+0,000+,+000_0,0,0,1_0,0.001_avg.csv', sep=';')
p4d = [df['LOD_phenotype_0'],df['LOD_phenotype_1'],df['LOD_phenotype_2'],df['LOD_phenotype_3']]
x = []
y = []
z = []

prev_sum = -1
count = 0
for i in range(len(df['LOD_phenotype_0'])):
    p4d = [df['LOD_phenotype_0'][i],df['LOD_phenotype_1'][i],df['LOD_phenotype_2'][i],df['LOD_phenotype_3'][i]]

    p3d = get_cartesian_from_barycentric(p4d, vertices)
    x.append(p3d[0])
    y.append(p3d[1])
    z.append(p3d[2])

print(p3d)
ax = fig.gca(projection='3d')
ax.plot(x, y, z, label='population dynamic')
ax.legend()

plt.show()
