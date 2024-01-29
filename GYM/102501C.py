N = int(input())
L = [False]*(N+1)
for i in range(N):
	x = int(input())
	if 0 <= x and x <= N:
		L[x] = True
i = 0
while L[i]: i += 1
print(i)
