import urllib
import sys

print("Scanning " + sys.argv[1])
contents = urllib.urlopen(sys.argv[1]).read()
ach_offs = contents.find("award-details")
names = []
while ach_offs != -1:
    ach_offs = contents.find("a href", ach_offs) + 8 + len("https://www.exophase.com/achievement/")
    ach_offs = contents.find("/", ach_offs) + 1
    ach_end = contents.find("/", ach_offs)
    names.append(contents[ach_offs:ach_end] + ".png")
    ach_offs = contents.find("award-details", ach_offs)
ach_offs = contents.find("https://m.exophase.com/android/awards/s")
links = []
while ach_offs != -1:
    ach_end = contents.find("\"", ach_offs)
    links.append(contents[ach_offs:ach_end].replace("/s/", "/l/"))
    ach_offs = contents.find("https://m.exophase.com/android/awards/s", ach_end)
idx = 0
for l in links:
    print("Downloading icon #" + str(idx))
    icon = urllib.urlopen(l).read()
    f = open(names[idx], "wb")
    f.write(icon)
    f.close()
    idx += 1