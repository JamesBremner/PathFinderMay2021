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


$(ODIR)/%.o: %.cpp $(DEPS)
	g++   -std=c++17 -c -o $@ $< $(INCS)

costs: $(OBJ) 
	g++ -o ../../bin/costs.exe $^ 


.PHONY: clean

clean:
	del /q obj