def printtarget(f, name, sdir, odir):
    f.write(name + ': ' + sdir + '/*.cpp \n')
    prcompmov(f, sdir, odir)

def prcompmov(f, sdir, odir):
    f.write('\tmkdir -p ' + odir + ' \n' \
            + '\t$(CC) $(CFLAGS) -c ' + sdir + '/*.cpp \n' \
            + '\tmv *.o ' + odir + '\n\n')
            
srcdir = 'src'
objdir = 'obj'
testdir = 'tests'
coresubdir = ('states','actions','state_transitions','observation_transitions' \
              ,'observations','policy','rewards','agents')             

f = open('Makefile', 'w')

# flags and directory wildcards
f.write('CC = g++\n' \
        'CFLAGS = -std=c++11 -g\n')

# include wildcard
#f.write('\nINCLUDE = ')
#for sd in coresubdir:
    #f.write('-Iinclude/core/' + sd + ' ')
#f.write('-Iinclude/util -Iinclude/fload -Iinclude/mdp\n\n')

# printing target rule for tests
f.write('tests: all.o ' + testdir + '/core/*.cpp ' \
        + testdir + '/mdp/*.cpp ' + testdir + '/file_loaders/*.cpp ' \
        + testdir + '/utilities/*.cpp\n')
f.write('\tmkdir -p tests/obj\n')
f.write('\t$(CC) $(CFLAGS) -c -I.. tests/core/*.cpp tests/mdp/*.cpp '\
        'tests/file_loaders/*.cpp tests/utilities/*.cpp tests/*.cpp\n')
f.write('\t$(CC) $(CFLAGS) -o perform_tests obj/*.o *.o\n')
f.write('\trm *.o\n\n')

# printing target rules for all object files
for sd in coresubdir:
    printtarget(f, sd + '.o', srcdir + '/core/' + sd, objdir)
printtarget(f, 'core.o', srcdir + '/core', objdir)
printtarget(f, 'util.o', srcdir + '/utilities', objdir)
printtarget(f, 'fload.o', srcdir + '/file_loaders', objdir)
printtarget(f, 'mdp.o', srcdir + '/mdp', objdir)
printtarget(f, 'pomdp.o', srcdir + '/pomdp', objdir)
printtarget(f, 'dec_mdp.o', srcdir + '/dec_mdp', objdir)
printtarget(f, 'dec_pomdp.o', srcdir + '/dec_pomdp', objdir)

f.write('make all.o: ')
for sd in coresubdir: f.write(sd + '.o ')
f.write('core.o util.o fload.o mdp.o pomdp.o dec_mdp.o dec_pomdp.o\n')

f.close()