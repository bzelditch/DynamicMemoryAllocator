########################################################################################
# CS 3410 PA4: Dynamic Memory Allocator (i.e., malloc)                                 #
#                                                                                      #
# Execute `make help` to enumerate all of the targets you can build.                   #
#                                                                                      #
# Do not, under any circumstances, edit this Makefile. For example, if you change the  #
# CFLAGS to include -O3 instead of -O0, what you perceive your performance to be will  #
# _not_ be what we test it against when it is graded.                                  #
#                                                                                      #
# You do not need to read anything below, but we have included excessive comments to   #
# hopefully de-mistify how make works.                                                 #
#                                                                                      #
########################################################################################
# Initial compilation and linking flag definitions.  Make works by recursively defining
# variables and even targets depending on what it determines needs to evaluated.  This
# is one of the most powerful features of make, however you sometimes need to bypass it.
# Use := to define a variable on that line.  You can also use ?= to set a variable only
# if it is not already defined (e.g. if a user has already defined it as a shell
# environment variable, you don't want to overwrite it).
CFLAGS := -O0 -Wall -Werror
LD_FLAGS =

# By default we will compile your custom heaplib.c since you will spend the majority of
# your time working on this file.
HEAPLIB_SOURCE = heaplib.c

# Make was designed to have targets (thing before the colon) have dependencies (things
# listed after the colon) that are files.  The reason is that when a given file in the
# list of dependencies is changed, make detects this (it uses the timestamp) and knows
# that any targets related to that file need to be changed.  PHONY is used to specify
# that you have a target, but it does not directly depend on any files.  Often times you
# can get away with not having PHONY, but it is hard to detect when this is the problem
# and you are better off using it.
.PHONY: all clean tests help
.PHONY: heaplib.o sizetask.o speedtask.o debug print-debug prof naive naive-debug naive-print-debug naive-prof

# The all target is what gets executed by default when you enter `make` without any
# target names.  Targets are all lines with a colon after them.  See the heaplib.o
# target below for why we are cleaning before we compile (this is non-standard, and
# somewhat defeats the purpose of make).
all: clean tests

########################################################################################
# Your custom heaplib additional compilation targets.                                  #
########################################################################################
# When you use #ifdef PRINT_DEBUG, this is what defines that.  In the rest of the file
# you will see many targets listed twice.  There is only one actual target, but the
# preceding lines are executing target-level definitions / modifications of variables.
print-debug: CFLAGS += -DPRINT_DEBUG
print-debug: debug

# There are four levels of debugging available in gcc and most compilers: 0,1,2,3.
# -g0 disables debugging symbols, and the default is -g2.  We choose -g3 for maximum
# information.  Both clang and icc support these levels.  Refer to
#     https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
# for more information than you'll ever want.
debug: CFLAGS += -g3
debug: clean tests

# For profiling, we need to both add compilation and linking flags.  It is generally not
# very meaningful to profile code compiled with debugging symbols.  Note only one target
# level variable can be defined / modified per line.
prof: CFLAGS += -pg
prof: LD_FLAGS += -pg
prof: clean tests

########################################################################################
# Naive targets: same as before except we compile heaplib_naive.c                      #
########################################################################################
# This is a target-level definition of HEAPLIB_SOURCE.  Since make evaluates variables
# lazily (as they are needed), the correct HEAPLIB_SOURCE will be used when compiling
# stages listed in the next section.  It still works if we used := at the top, but in
# this scenario it was only defined once, rather than overwritten.
naive: HEAPLIB_SOURCE = heaplib_naive.c
naive: clean tests
# Compile with extra detailed debugging information.
naive-debug: CFLAGS += -g3
naive-debug: naive
# Compile with debug information, and define PRINT_DEBUG for the c file.
naive-print-debug: CFLAGS += -DPRINT_DEBUG
naive-print-debug: naive-debug
# As before, gprof requires a linking flag of -pg, and we omit the debugging symbols.
naive-prof: CFLAGS += -pg
naive-prof: LD_FLAGS += -pg
naive-prof: naive

########################################################################################
# The actual compilation.                                                              #
########################################################################################
# The following compiles the previously defined HEAPLIB_SOURCE into an object file using
# the -c flag of gcc.  We expand the CFLAGS that have been modified by various targets
# to achieve the compilation goal we desire.  Finally, we must include -o and specify
# heaplib.o ($@ expands to the name of the target) because we will either be compiling
# heaplib.c (which would normally be heaplib.o) _or_ heaplib_naive.c (which by default
# would compile into heaplib_naive.o).  Our later targets expect heaplib.o.
heaplib.o:
	gcc -c $(CFLAGS) $(HEAPLIB_SOURCE) -o $@

sizetask.o:
	gcc -c $(CFLAGS) sizetask.c -o $@

speedtask.o:
	gcc -c $(CFLAGS) speedtask.c -o $@

# Depend on all of the test files we seek to compile so that all of them will be built.
tests: heaptest test_c test_t test_u

# When you compile executables, you always include the linking (LD_FLAGS) last.  Each
# target below uses $+, which expands to all of the dependencies.  For example, in the
# heaptest target $+ expands to: heaplib.o heaptest.c
heaptest: heaplib.o heaptest.c
	gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)

test_c: heaplib.o test_c.c
	gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)

test_t: heaplib.o speedtask.o test_t.c
	gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)

test_u: heaplib.o sizetask.o test_u.c
	gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)

# As a general courtesy, it is common practice to define a clean target that performs
# cleanup for the user.  What is cleaned and how many clean targets you define varies
# by philosophy, but it is good practice to define at least one.
clean:
	rm -f *.o heaptest test_c test_t test_u

# Sometimes people also define help, but it is much less common.  You may have noticed
# that all the commands that actually get executed by make (gcc in this file) print to
# the screen.  This is the default behavior to allow transparency.  If you want to
# suppress the printing, you put an @ in front of the line you want to suppress so that
# it is just executed.  Since this is printing a help message, the command echoing
# behavior of make is less than helpful...
help:
	@echo -e "\nYour custom heaplib.c implementation targets:"
	@echo "  make                   : heaplib.c"
	@echo "  make prof              : heaplib.c with profiling symbols"
	@echo "  make debug             : heaplib.c with debugging symbols"
	@echo "  make print-debug       : heaplib.c with debugging symbols and PRINT_DEBUG"
	@echo -e "\nThe naive implementation targets:"
	@echo "  make naive             : heaplib_naive.c"
	@echo "  make naive-prof        : heaplib_naive.c with profiling symbols"
	@echo "  make naive-debug       : heaplib_naive.c with debugging symbols"
	@echo "  make naive-print-debug : heaplib_naive.c with debugging symbols and PRINT_DEBUG"
	@echo -e "\nAll targets compile the heaptest, test_c, test_t, and test_u executables. You"
	@echo "can use tab completion for make targets on the course VM.  It will also list"
	@echo "files in the current directory, but will work as long as you complete to a"
	@echo "valid target."
	@echo -e "\n  - For non-default targets, type make and don't hit enter."
	@echo "  - Hit tab twice to list all potential completions."
	@echo "  - When you type enough for only one to be possible it will be completed."
	@echo "  - [hit enter]"
	@echo -e "\nNOTE: There are other targets that will show up with tab completion."
	@echo "      These targets should not be compiled (e.g. heaptest, test_c). Use the"
	@echo -e "      targets we have listed above.\n"

# So in the end, this is a rather non-standard Makefile since we are varying what is
# compiled.  If your ultimate goal was to create just one executable (say heaptest),
# then you would just do something like below.  The only other gotchas with Make are
# that you have to use TAB characters in targets (not spaces), as well as a common C/C++
# mistake is to not include the header files in the target dependencies.  We do not use
# the header file (heaplib.h) above because you should NOT be changing it.  But if
# you are writing your own project, then it is likely you will be adding new code in
# the header as you develop, and if it is not explicitly listed in one of the targets
# then the new changes may not be brought in.
#
# .PHONY: all test
#
# all: heaptest
#
# heaptest: heaplib.o heaplib.h heaptest.c
#     gcc $(CFLAGS) $+ -o $@ $(LD_FLAGS)
#
# # $< is the first dependency listed
# heaplib.o: heaplib.c
#     gcc -c $(CFLAGS) $< -o $@
#
# clean:
#     rm -f *.o heaptest
#
# The following is an extremely useful though somewhat hard to search for resource on
# what the different automatic variables are:
#
#     https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#
# Happy making, go write your own recipe!
