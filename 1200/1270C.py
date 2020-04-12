from math import log2

t = int(input())

for _ in range(t):
	n = int(input())
	a = list(map(int, input().split()))
	s = sum(a)
	sm = 0
	for ai in a:
		sm = sm ^ ai
	sm *= 2
	pm = 2**int(log2(10**17))
	# print(bin(pm))
	obj = pm + sm
	add = (obj - s) // 2
	if s%2 == 1: add -= 1
	add2 = pm+1 if s%2==1 else pm
	print(3)
	print(add, add, add2)
	# print(sm)
	# print("o", bin(obj), bin(s+2*add), add2==pm+1)
	# print(bin(s+2*add+add2), bin(sm ^ (2*add2)))
	# print(s+2*add+add2, sm ^ (2*add2))