import random as random
m = ["fiat",
"renault",
"volvo",
"kia",
"audi",
"lexus",
"scoda",
"honda",
"mazda",
"ford",
"toyota",
"bmw",
"lada"]

c = [
    "russia",
    "spain",
    "italy",
    "france",
    "germany",
    "UK",
    "USA",
    "china",
    "austria",
    "belarus"]
col = [
    "red",
    "green",
    "blue",
    "black",
    "white",
    "gray",
    "purple"]
f = open("text.txt", "w")
for i in range(100):
    mark = m[random.randint(0, 12)]
    country = c[random.randint(0, 9)]
    price = random.uniform(10.0, 100.0)
    color = col[random.randint(0, 6)]
    is_old = random.randint(0, 1)
    f.write(mark + "\n"); f.write(country + "\n");
    f.write("{:.2f}\n".format(price)); f.write(color + "\n")
    if is_old == 1:
        f.write("old\n")
        f.write(str(random.randint(1950, 2015)))
        f.write("\n")
        f.write(str(random.randint(10000, 1000000)))
        f.write("\n")
        f.write(str(random.randint(0, 5)))
        f.write("\n")
        f.write(str(random.randint(0, 3)))
        f.write("\n")
    else:
        f.write("new\n")
        f.write(str(random.randint(5, 20)))
        f.write("\n")
    f.write("\n")
f.close()
