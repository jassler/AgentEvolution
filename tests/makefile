CC = g++
INC = -Iinc
CFLAGS = -std=c++17 -O3

ODIR = bin
OBJDIR = $(ODIR)/objs
OFILE = $(ODIR)/agentevolver
SDIR = ../AgentEvolution

_OBJS = agent.o population.o randwrap.o argparser.o test.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

# main output
$(OFILE): $(OBJS)
	$(CC) -o $(OFILE) $(CFLAGS) $(OBJS)

# all object files only depend on their corresponding .cpp-files
# mostly gotten from here: https://stackoverflow.com/questions/1814270
$(OBJDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

$(OBJDIR)/test.o: test.cpp
	@mkdir -p $(@D)
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)



.PHONY: clean
clean:
	rm -f $(OFILE) $(OBJDIR)/*.o