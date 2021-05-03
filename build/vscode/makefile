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

_FARMS = farms.o cPathFinder.o cFarmPathFinder.o \
	geo.o geoPolygon.o
FARMS = $(patsubst %,$(ODIR)/%,$(_FARMS))

_MAZES = mazes.o cPathFinder.o cMazePathFinder.o 
MAZES = $(patsubst %,$(ODIR)/%,$(_MAZES))

$(ODIR)/%.o: %.cpp $(DEPS)
	g++   -std=c++17 -c -o $@ $< $(INCS)

costs: $(OBJ) 
	g++ -o ../../bin/costs.exe $^ 

turns: $(TURNS)
	g++ -o ../../bin/turns.exe $^ 	

hills: $(HILLS)
	g++ -o ../../bin/hills.exe $^ 

farms: $(FARMS)
	g++ -o ../../bin/farms.exe $^ 

mazes: $(MAZES)
	g++ -o ../../bin/mazes.exe $^ 

.PHONY: clean

clean:
	del /q obj