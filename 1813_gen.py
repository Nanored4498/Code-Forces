import random

# N = 4
# Q = 100
# M = 2000
# D = [40, 100, 50, 200]
# print(N, Q, M)
# print(4, 8, 3, 10)
# print(*D)
# print(10, 15, 20)
# print(20, 40, 50)
# print(12, 15, 16)
# print(30, 60, 70)

# for _ in range(M):
#     u = random.randint(1, N)
#     p = random.randint(1, D[u-1])
#     print(u, p)

N = 3
T = 2000
T2 = 1000
Q = 45
M = 2*(T+T2+T)
D = [120, 40, 80]
print(N, Q, M)
print(10, 4, 6)
print(*D)
print(19, 48, 45)
print(6, 15, 45)
print(10, 25, 45)

A = [random.randint(1, D[0]) for _ in range(T)]
B = [random.randint(1, D[1]) for _ in range(T2)]
C = [random.randint(1, D[2]) for _ in range(T)]
for p in A:
	print(1, p)
for p in B:
	print(2, p)
for p in C:
	print(3, p)
for p in A:
	print(1, p)
for p in B:
	print(2, p)
for p in A:
	print(1, p)