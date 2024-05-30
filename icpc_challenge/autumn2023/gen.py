import numpy as np

N = 2
K = 3
T = 10
R = 4
print(N, K, T, R)
for _ in range(T*K*R):
    # print(*(round(np.exp(np.random.rand()*np.log(1e4)), 4) for _ in range(N)))
    print(*(3 for _ in range(N)))
for _ in range(K*R*N):
    print(*(round(np.random.rand()*(-2), 4) for _ in range(N)))

jobs = []
for n in range(N):
    # j = np.random.randint(3, 7)
    j = 3
    td = T//j
    for i in range(j):
        # size = int((0.6 + 0.4*np.random.rand()) * td * R * np.log2(101))
        size = 192*K*(2*R+1) if n == 0 else 192*K*3
        jobs.append((len(jobs), size, n, i*td, td))
    
print(len(jobs))
for j in jobs:
    print(*j)