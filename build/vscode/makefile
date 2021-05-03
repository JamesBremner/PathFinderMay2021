#source file search paths
VPATH = ../../src 

# compiler include search paths
INCS = \
	-I../../../boost/boost1_72

DEPS = cPathFinder.h

# folder for .o files
ODIR=./obj

# sources
_OBJ = costs.o cPathFinder.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TURNS = turn.o cPathFinder.o cTurnPathFinder.o
TURNS = $(patsubst %,$(ODIR)/%,$(_TURNS))

_HILLS = hills.o cPathFinder.o cHillPathFinder.o
HILLS = $(patsubst %,$(ODIR)/%,$(_HILLS))

$(ODIR)/%.o: %.cpp $(DEPS)
	g++   -std=c++17 -c -o $@ $< $(INCS)

costs: $(OBJ) 
	g++ -o ../../bin/costs.exe $^ 

turns: $(TURNS)
	g++ -o ../../bin/turns.exe $^ 	

hills: $(HILLS)
	g++ -o ../../bin/hills.exe $^ 

.PHONY: clean

clean:
	del /q obj