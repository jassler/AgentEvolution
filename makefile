CC = g++
INC = -Iinc
CFLAGS = -std=c++17 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
# -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2

ODIR = bin
OBJDIR = $(ODIR)/objs
OFILE = $(ODIR)/agentevolver
SDIR = AgentEvolution

_OBJS = main.o agent.o argparser.o matrix.o population.o randwrap.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

# main output
$(OFILE): $(OBJS)
	$(CC) -o $(ODIR)/agentevolver $(CFLAGS) $(OBJS)

# all object files only depend on their corresponding .cpp-files
# mostly gotten from here: https://stackoverflow.com/questions/1814270
$(OBJDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $(INC) $(CFLAGS) -o $@ $<

# according to GNU, 'make print' should display all cpp files that have changed.
# not really working though (wrong directory?)
# https://www.gnu.org/software/make/manual/make.html#Wildcard-Examples
print: $(SDIR)/*.cpp
	lpr -p $?
	touch $(SDIR)/print

.PHONY: test
test:
	make -C ./tests


.PHONY: clean
clean:
	rm -f $(OFILE) $(OBJDIR)/*.o