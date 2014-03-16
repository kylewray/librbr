def printtarget(f, name, sdir, odir):
    f.write(name + '.o: ' + sdir + '/*.cpp \n' \
            + '\tmkdir -p ' + odir + ' \n' \
            + '\t$(CC) $(CFLAGS) -c ' + sdir + '/*.cpp \n' \
            + '\tmv *.o ' + odir + '\n\n')
            
srcdir = 'SRC_DIR'
objdir = 'OBJ_DIR'

coresubdir = ('states','actions','state_transitions','observation_transitions' \
              ,'observations','policy','rewards','agents')
      
f = open('Makefile', 'w')

# flags and directory wildcards
f.write('CC = g++ \n' \
        'CFLAGS = -std=c++11 -g\n' \
        'SDIR = src\n' \
        'TDIR = tests\n' \
        'ODIR  = obj\n\n' \
        'CORE_SRC_DIR = $(SDIR)/core\n' \
        'CORE_OBJ_DIR = $(ODIR)/core\n' \
        'UTIL_SRC_DIR = $(SDIR)/utilities\n' \
        'UTIL_OBJ_DIR = $(ODIR)/utilities\n' \
        'FLOAD_SRC_DIR = $(SDIR)/file_loaders\n' \
        'FLOAD_OBJ_DIR = $(ODIR)/file_loaders\n' \
        'MDP_SRC_DIR = $(SDIR)/mdp\n' \
        'MDP_OBJ_DIR = $(ODIR)/mdp\n')        
for sd in coresubdir:
    dirU = sd.upper() + "_"
    f.write(dirU + srcdir + ' = $(CORE_SRC_DIR)/' + sd + '\n' \
            + dirU + objdir + ' = $(CORE_OBJ_DIR)/' + sd + '\n')

# include wildcard
#f.write('\nINCLUDE = ')
#for sd in coresubdir:
    #f.write('-Iinclude/core/' + sd + ' ')
#f.write('-Iinclude/util -Iinclude/fload -Iinclude/mdp\n\n')

# printing target rule for tests
#f.write('tests: ') 
#f.write('$(CORE_OBJ_DIR)/*.o $(UTIL_OBJ_DIR)/*.o $(FLOAD_OBJ_DIR)/*.o ' \
        #'$(MDP_OBJ_DIR)/*.o ')
#for sd in coresubdir:
    #f.write('$(' + sd.upper() + '_' + objdir + ')/*.o ')
#f.write('\n')
#for sd in coresubdir:
    #printtarget(f, sd, 'tests/core/' + sd, 'tests/obj')

# printing target rules for all object files
f.write('\n')
for sd in coresubdir:
    du = sd.upper() + '_'
    printtarget(f, sd, '$(' + du + srcdir + ')', '$(' + du + objdir +')')
printtarget(f, 'core', '$(CORE_SRC_DIR)', '$(CORE_OBJ_DIR)')
printtarget(f, 'util', '$(UTIL_SRC_DIR)', '$(UTIL_OBJ_DIR)')
printtarget(f, 'mdp', '$(MDP_SRC_DIR)', '$(MDP_OBJ_DIR)')
printtarget(f, 'fload', '$(FLOAD_SRC_DIR)', '$(FLOAD_OBJ_DIR)')

f.write('make all.o: ')
for sd in coresubdir:
    f.write(sd + '.o ')
f.write('core.o ')
f.write('util.o ')
f.write('fload.o ')
f.write('mdp.o')
f.write('\n')

f.close()