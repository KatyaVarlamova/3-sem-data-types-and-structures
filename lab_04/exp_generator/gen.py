import random as random
number = 1000000
f = open("text.txt", "w")
open_sc = ['(', '{', '[']
close_sc = [')', '}', ']']
sp = []
for i in range(number):
    r = random.randint(0, 2)
    f.write(open_sc[r])
    sp.append(close_sc[r])
for i in range(number - 1, -1, -1):
    f.write(sp[i])
f.close()
