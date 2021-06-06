t = int(input())
for _ in range(t):
	n = int(input())
	s = input()
	c = s.count('0')
	if c&1 and c != 1: print('ALICE')
	else: print('BOB')
	