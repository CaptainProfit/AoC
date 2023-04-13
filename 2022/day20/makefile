cc=g++
cflags=-I.
ODIR=obj
DEPS=orderedTree.hpp
_OBJ = day20_1.o orderedTree.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

day20_1.exe: 
	if (Test-Path -Path obj) {mkdir 'obj'}
	g++ -g *.o -o day20_1.exe

$(ODIR)/*.o: *.cpp $(DEPS)
	$(cc) -g  -c -o $@ $^ $(cflags)0

#g++ orderedTree.cpp -c