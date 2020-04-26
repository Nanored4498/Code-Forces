T = int(input())

for _ in range(T):
	t = input()
	one = True
	for j in range(1, len(t)):
		if t[0] != t[j]:
			one = False
			break
	if one: print(t)
	else: print('01'*len(t))