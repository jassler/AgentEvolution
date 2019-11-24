CC = g++
INC = -I../src
CFLAGS = -std=c++17 -O3
TESTFLAGS = -lgtest -lpthread

ODIR = bin
OBJDIR = $(ODIR)/objs
OFILE = test
SDIR = ../src

# _TESTOBJ = test.o test_argparser.o test_matrix.o test_agent.o test_population.o
# _SRCOBJ = agent.o matrix.o population.o randwrap.o argparser.o
_TESTOBJ = test.o test_matrix.o test_agent.o
_SRCOBJ = matrix.o matrixexception.o agent.o randwrap.o args.o
_OBJS = $(_SRCOBJ) $(_TESTOBJ)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

# main output
$(OFILE): $(OBJS)
	$(CC) -o $(OFILE) $(CFLAGS) $(TESTFLAGS) $(OBJS)

# all object files only depend on their corresponding .cpp-files
# mostly gotten from here: https://stackoverflow.com/questions/1814270
$(OBJDIR)/%.o: $(SDIR)/%.cpp $(SDIR)/%.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(INC) -o $@ $<

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(INC) -o $@ $<


.PHONY: clean
clean:
	rm -f $(OFILE) $(OBJDIR)/*.o
