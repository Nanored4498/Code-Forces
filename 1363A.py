t = int(input())
for _ in range(t):
	n, x = map(int, input().split())
	a = list(map(int, input().split()))
	ne, no = 0, 0
	for ai in a:
		if ai % 2 == 0: ne += 1
		else: no += 1
	if no == 0: print("No")
	elif x%2==0 and ne==0: print("No")
	elif x==n and no%2==0: print("No")
	else: print("Yes")