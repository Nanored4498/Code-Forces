n, m = map(int, input().split())
A = []
for _ in range(n):
	A.append(list(map(int, input().split())))

res = []
for i in range(n):
	for j in range(m):
		if A[i][j] == 1:
			if i > 0 and j > 0 and A[i-1][j] == A[i-1][j-1] == A[i][j-1] == 1:
				res.append((i,  j))
			elif i > 0 and j < m-1 and A[i-1][j] == A[i-1][j+1] == A[i][j+1] == 1:
				res.append((i, j+1))
			elif i < n-1 and j < m-1 and A[i+1][j] == A[i+1][j+1] == A[i][j+1] == 1:
				res.append((i+1, j+1))
			elif i < n-1 and j > 0 and A[i+1][j] == A[i+1][j-1] == A[i][j-1] == 1:
				res.append((i+1, j))
			else:
				print(-1)
				exit(0)
print(len(res))
for i, j in res:
	print(i, j)