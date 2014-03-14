srcdir = '_SRC_DIR'
objdir = '_OBJ_DIR'

coresubdir = ('states','actions','state_transitions','')
      
f = open('Makefile', 'w')
f.write('CC = g++ \n' \
        'CFLAGS = -std=c++11 -g\n' \
        'SDIR = src\n' \
        'ODIR  = obj\n' \
        'SCDIR = $(SDIR)/core\n' \
        'OCDIR = $(ODIR)/core\n\n')
        
for dir in coresubdir:
    dirU = dir.upper()
    f.write(dirU + srcdir + ' = $(SCDIR)/' + dir + '\n' \
            + dirU + objdir + ' = $(OCDIR)/' + dir + '\n')
            
f.write('\n')
for dir in coresubdir:
    dirU = dir.upper()
    f.write(dir + '.o: $(' + dirU + srcdir + ')/*.cpp \n' \
            + '\tmkdir -p $(' + dirU + objdir + ') \n' \
            + '\t$(CC) $(CFLAGS) -c $(' + dirU + srcdir + ')/*.cpp \n' \
            + '\tmv *.o $(' + dirU + objdir + ')\n\n')

f.write('make all: ')
for dir in coresubdir:
    f.write(dir + '.o ')
f.write('\n')

f.close()