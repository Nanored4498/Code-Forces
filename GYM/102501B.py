N = int(input())
L = []
for _ in range(N):
	L.append(input())
L.sort()
i, j = 0, 1
while i < N:
	while j < N and L[j] == L[i]:
		j += 1
	if 2*(j-i) > N: break
	i = j
if i < N: print(L[i])
else: print("NONE")