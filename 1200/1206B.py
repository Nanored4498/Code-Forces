n = int(input())
a = list(map(int, input().split()))
neg = 0
res = 0
zero = False
for v in a:
	if v == 0: zero, res = True, res+1
	elif v < 0: res, neg = res-v-1, neg+1
	else: res += v-1
if neg % 2 == 1 and not(zero): res += 2
print(res)