#!/usr/bin/python3
import glob

lista = glob.glob('actions/*.yml')
print(lista)
contagem = 0
alllines = ''
lines = []
first = True
for a in lista:
    print(a)
    f = open(a,'r')
    # alllines += (f.read() + '\n\n\n\n')


    if first:
        first = False
        lines += 'name: OF'
        lines += ''
        lines += f.readlines()[1:]
    else:
        lines += f.readlines()[23:]
    # lines.append(f.readlines()[2:])
    lines += '\n\n'
    lines += '# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n\n'
    # lines.append('\n\n\n\n')
    f.close() 

all = open('of.yml','w')
# all.write(alllines)
print(lines)
all.writelines(lines)
all.close()

print (alllines)