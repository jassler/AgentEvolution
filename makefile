CC = g++
INC = -Iinc
CFLAGS = -std=c++17

ODIR = bin
OBJDIR = $(OUTBIN)/objs
SDIR = AgentEvolution

_OBJS = main.o agent.o population.o
OBJS = $(pathsubst %,$(OBJDIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OBJDIR)/*.o