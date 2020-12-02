import random as random
RAND = False
MAXLEN = 5
COUNT = 500

alp = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"]

def to_word(n):
    s = ""
    while n != 0:
        s += alp[n % 26]
        n //= 26
    while (len(s) != MAXLEN):
        s += "a"
    g = ''.join(reversed(s))
    return g

def to_rand_word():
    length = random.randint(1, MAXLEN)
    s = ""
    letter_min = 97
    letter_max = 122
    for j in range(length):
        letter = random.randint(letter_min, letter_max)
        s += chr(letter)
    return s

        
f = open("file.txt", "w")
for i in range(COUNT):
    if RAND == True:
        s = to_rand_word()
    else:
        s = to_word(i)
    f.write(s + "\n")
f.close()

##n = 1000
##for i in range(n):
##    r = i + 1#random.randint(0, 100000)
##    f.write(str(r) + "\n")
##f.close()
