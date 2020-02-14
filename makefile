CC = g++
INC = -I./src
CFLAGS = -std=c++17 -O3 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
# -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2

ODIR = bin
OBJDIR = $(ODIR)/objs
OFILE = $(ODIR)/agentevolver
SDIR = src

POPULATION=1000
GENSIZE=3
PHENSIZE=3
PAYOFF="{{0,1,-1},{-1,0,1},{1,-1,0}}"
AGENTSTART="{0.333,0.333,0.333}"
GENERATIONS=10000
OPPONENTS=8
GENOME_PROB=0.0001
MATRIX_PROB=0

LOG_DEBUG=0
LOG_PROGRESS=0

DEFINES=-DPOPULATION=$(POPULATION) -DGENSIZE=$(GENSIZE) -DPHENSIZE=$(PHENSIZE) -DPAYOFF=$(PAYOFF) -DAGENTSTART=$(AGENTSTART) -DGENOME_PROB=$(GENOME_PROB) -DMATRIX_PROB=$(MATRIX_PROB) -DGENERATIONS=$(GENERATIONS) -DOPPONENTS=$(OPPONENTS) -DLOG_DEBUG=$(LOG_DEBUG) -DLOG_PROGRESS=$(LOG_PROGRESS)

_OBJS = main.o agent.o matrix.o matrixexception.o randwrap.o world.o fileagent.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

# main output
$(OFILE): $(OBJS)
	$(CC) -o $(OFILE) $(CFLAGS) $(OBJS) $(DEFINES)

# all object files only depend on their corresponding .cpp-files
# mostly gotten from here: https://stackoverflow.com/questions/1814270
$(OBJDIR)/%.o: $(SDIR)/%.cpp $(SDIR)/%.hpp
	@mkdir -p $(@D)
	$(CC) -c $(INC) $(CFLAGS) $(DEFINES) -o $@ $<

# according to GNU, 'make print' should display all cpp files that have changed.
# not really working though (wrong directory?)
# https://www.gnu.org/software/make/manual/make.html#Wildcard-Examples
print: $(SDIR)/*.cpp
	lpr -p $?
	touch $(SDIR)/print

.PHONY: test
test:
	cd tests
	make


.PHONY: clean
clean:
	rm -f $(OFILE) $(OBJDIR)/*.o

.PHONY: cleanall
cleanall:
	rm -rf $(ODIR)/*

.PHONY: loginput
loginput:
	: ODIR   = $(ODIR)
	: OBJDIR = $(OBJDIR)
	: OFILE  = $(OFILE)
	: SDIR   = $(SDIR)

	: POPULATION  = $(POPULATION)
	: GENSIZE     = $(GENSIZE)
	: PHENSIZE    = $(PHENSIZE)
	: PAYOFF      = $(PAYOFF)
	: AGENTSTART  = $(AGENTSTART)
	: GENERATIONS = $(GENERATIONS)
	: OPPONENTS   = $(OPPONENTS)
	: GENOME_PROB = $(GENOME_PROB)
	: MATRIX_PROB = $(MATRIX_PROB)

	: LOG_DEBUG    = $(LOG_DEBUG)
	: LOG_PROGRESS = $(LOG_PROGRESS)