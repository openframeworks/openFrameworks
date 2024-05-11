#!/usr/bin/python3
import glob

lista = glob.glob('actions/*.yml')
print(lista)
contagem = 0
alllines = ''
for a in lista:
    print(a)
    f = open(a,'r')
    alllines += (f.read() + '\n\n\n\n')
    f.close() 

all = open('of.yml','w')
all.write(alllines)
all.close()

print (alllines)