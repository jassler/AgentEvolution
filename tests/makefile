CC = g++
INC = -I../src
CFLAGS = -std=c++17 -O3
TESTFLAGS = -lgtest -lpthread

ODIR = bin
OBJDIR = $(ODIR)/objs
OFILE = test.out
SDIR = ../src

POPULATION=1000
GENSIZE=3
PHENSIZE=3
PAYOFF="{{0,1,1},{-1,0,1},{-1,-1,0}}"
AGENTSTART="{0.333,0.333,0.333}"
GENOME_PROB=0.0001
MATRIX_PROB=0
GENERATIONS=10000

DEFINES=-DPOPULATION=$(POPULATION) -DGENSIZE=$(GENSIZE) -DPHENSIZE=$(PHENSIZE) -DPAYOFF=$(PAYOFF) -DAGENTSTART=$(AGENTSTART) -DGENOME_PROB=$(GENOME_PROB) -DMATRIX_PROB=$(MATRIX_PROB) -DGENERATIONS=$(GENERATIONS)

# _TESTOBJ = test.o test_argparser.o test_matrix.o test_agent.o test_population.o
# _SRCOBJ = agent.o matrix.o population.o randwrap.o argparser.o
_TESTOBJ = test.o test_matrix.o test_agent.o test_world.o
_SRCOBJ = matrix.o matrixexception.o agent.o randwrap.o world.o fileagent.o
_OBJS = $(_SRCOBJ) $(_TESTOBJ)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))



# main output
$(OFILE): $(OBJS)
	$(CC) -o $(OFILE) $(CFLAGS) $(TESTFLAGS) $(OBJS) $(DEFINES)

# all object files only depend on their corresponding .cpp-files
# mostly gotten from here: https://stackoverflow.com/questions/1814270
$(OBJDIR)/%.o: $(SDIR)/%.cpp $(SDIR)/%.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(INC) $(DEFINES) -o $@ $<

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(INC) $(DEFINES) -o $@ $<


.PHONY: clean
clean:
	rm -f $(OFILE) $(OBJDIR)/*.o
