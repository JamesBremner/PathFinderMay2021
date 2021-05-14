#source file search paths
VPATH = ../../src 

# compiler include search paths
INCS = \
	-I../../../boost/boost1_72

DEPS = cPathFinder.h

WEXINC = \
	-I../../../windex/include

WEXLIBS = \
	-lstdc++fs -lgdiplus -lgdi32 -lcomdlg32

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

_SPANS = spans.o cPathFinder.o cMazePathFinder.o 
SPANS = $(patsubst %,$(ODIR)/%,$(_SPANS))

_SALES = sales.o cPathFinder.o cSalesPathFinder.o 
SALES = $(patsubst %,$(ODIR)/%,$(_SALES))

_COMPS = comps.o cPathFinder.o
COMPS = $(patsubst %,$(ODIR)/%,$(_COMPS))

_GUI = pathgui.o cPathFinder.o
GUI = $(patsubst %,$(ODIR)/%,$(_GUI))

$(ODIR)/%.o: %.cpp $(DEPS)
	g++   -std=c++17 -g -c -o $@ $< $(INCS) $(WEXINC)

costs: $(OBJ) 
	g++ -o ../../bin/costs.exe $^ 

turns: $(TURNS)
	g++ -o ../../bin/turns.exe $^ 	

hills: $(HILLS)
	g++ -o ../../bin/hills.exe $^ 

farms: $(FARMS)
	g++ -o ../../bin/farms.exe $^ 

mazes: $(MAZES)
	g++ -g -o ../../bin/mazes.exe $^ 

spans: $(SPANS)
	g++ -o ../../bin/spans.exe $^ 

sales: $(SALES)
	g++ -o ../../bin/sales.exe $^ 

comps: $(COMPS)
	g++ -o ../../bin/comps.exe $^ 

gui: $(GUI)
	g++ -o ../../bin/gui.exe $^ $(WEXLIBS)

.PHONY: clean

clean:
	del /q obj