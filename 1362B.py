t = int(input())
for _ in range(t):
	n = int(input())
	S = list(map(int, input().split()))
	if n == 1:
		print(-1)
		continue
	if n == 2:
		print(S[0]^S[1])
		continue
	S.sort()
	ans = -1
	for k in range(1, 1024):
		R = sorted([x^k for x in S])
		ans = k
		for i in range(n):
			if S[i] != R[i]:
				ans = -1
				break
		if ans > 0: break
	print(ans)

