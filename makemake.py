""" The MIT License (MIT)
 
 Copyright (c) 2013 Kyle Wray and Luis Pineda
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
"""

import os
import os.path


def printtarget(f, name, sdir, odir):
    """ Prints a target rule to the specified file.
    
        Parameters:
            f    -- The file where the output will be printed to. 
            name -- The name of the target rule. 
            sdir -- The source directory to compile this rule.
            odir -- The object directory to store the .o files.
    """

    f.write(name + ': ' + sdir + '/*.cpp \n')
    prcompmov(f, sdir, odir)


def prcompmov(f, sdir, odir):
    """ Prints a set of bash comands to create an object directory (if it 
        doesn't exists), compile all source files in a source directory and
        move the object files to the object directory.

        Parameters:
            f    -- The file where the output will be printed to. 
            sdir -- The directory where the source files are stored.
            odir -- The directory to where the .o files will be stored.
    """

    f.write('\tmkdir -p ' + odir + ' \n' \
            + '\t$(CC) $(CFLAGS) -c ' + sdir + '/*.cpp \n' \
            + '\tmv *.o ' + odir + '\n\n')


srcdir = 'librbr/src'
objdir = 'librbr/obj'
testdir = 'librbr_tests'
coresubdir = ['states', 'actions', 'state_transitions', \
            'observation_transitions', 'observations', 'policy', \
            'rewards','agents']

f = open('Makefile', 'w')


# Test if the 'librbr/obj', 'librbr_tests/obj', and 'tmp'
# directories exist and make them if they do not.
directories = ['librbr/obj', 'librbr_tests/obj', 'librbr_tests/tmp']
for d in directories:
    if not os.path.exists(d):
        os.makedirs(d)


# Printing flags and directory wildcards.
f.write('CC = g++\n' \
        'CFLAGS = -std=c++11 -g\n' \
        'COINFLAGS = `pkg-config --cflags --libs Coin` '
        '`pkg-config --cflags --libs clp` '
        '`pkg-config --cflags --libs osi` '
        '`pkg-config --libs coinutils` '
        '`pkg-config --cflags --libs osi-clp`\n')

# Printing target rule for tests.
f.write('tests: all.o ' + testdir + '/core/*.cpp ' + \
        testdir + '/mdp/*.cpp ' + testdir + '/file_loaders/*.cpp ' + \
        testdir + '/utilities/*.cpp\n')
f.write('\tmkdir -p ' + testdir + '/obj\n')
f.write('\t$(CC) $(CFLAGS) -c -I.. ' + testdir + '/core/*.cpp ' + \
        testdir + '/mdp/*.cpp ' + testdir + '/pomdp/*.cpp ' + \
        testdir + '/file_loaders/*.cpp ' + testdir + '/utilities/*.cpp ' + \
        testdir + '/*.cpp\n')
f.write('\t$(CC) $(CFLAGS) $(COINFLAGS) -o perform_tests ' + \
        objdir + '/*.o *.o\n')
f.write('\trm *.o\n\n')

# Printing target rules for all object files.
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
for sd in coresubdir: 
    f.write(sd + '.o ')
f.write('core.o util.o fload.o mdp.o pomdp.o dec_mdp.o dec_pomdp.o\n')

f.close()

