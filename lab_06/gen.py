import random as random
f = open("file.txt", "w")
n = 10
maxlen = 15
letter_min = 97
letter_max = 122
for i in range(n):
    l = random.randint(1, maxlen)
    s = ""
    for j in range(l):
        letter = random.randint(letter_min, letter_max)
        s += chr(letter)
    f.write(s + "\n")
f.close()
##n = 1000
##for i in range(n):
##    r = i + 1#random.randint(0, 100000)
##    f.write(str(r) + "\n")
##f.close()
