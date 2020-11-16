import random as random
f = open("file.txt", "w")
n = 10000000
for i in range(n):
    r = random.randint(0, 10)
    f.write(str(r) + "\n")
f.close()
