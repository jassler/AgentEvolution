CC = g++
INC = -I../AgentEvolution
CFLAGS = -std=c++17 -O3
TESTFLAGS = -lgtest -lpthread

ODIR = bin
OBJDIR = $(ODIR)/objs
OFILE = $(ODIR)/agentevolver_test
SDIR = ../AgentEvolution

_TESTOBJ = test.o test_matrix.o test_agent.o test_population.o
_SRCOBJ = agent.o matrix.o population.o randwrap.o argparser.o
_OBJS = $(_SRCOBJ) $(_TESTOBJ)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

# main output
$(OFILE): $(OBJS)
	$(CC) -o $(OFILE) $(CFLAGS) $(TESTFLAGS) $(OBJS)

# all object files only depend on their corresponding .cpp-files
# mostly gotten from here: https://stackoverflow.com/questions/1814270
$(OBJDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(OFILE) $(OBJDIR)/*.o