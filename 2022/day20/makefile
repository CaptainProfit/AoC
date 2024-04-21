#[UTF-8 ru/en]
cc=g++
cflags=-g -I . -Wall -pedantic -fdiagnostics-color=always -pass-exit-codes
ODIR=obj
DEPS=orderedTree.hpp
_OBJ = day20_1.o orderedTree_interface.o orderedTree_rebalance.o orderedTree_rotates.o 
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

#target: 
target obj/day20_1.exe: $(OBJ)
	echo "<<<<target"
	$(cc) $(cflags) $^ -o obj/day20_1.exe

#objects:
#%.o: no
# $(OBJ): %.cpp $(DEPS) no
#
$(ODIR)/%.o: %.cpp $(DEPS)
	echo "<<<< compiling 	$@ by $^"
	mkdir -p obj
	$(cc) -c $(cflags) $< -o $@

#g++ orderedTree.cpp -c

.PHONY: clean rebuild remake target help
#clean: 
clean:
	echo "<<<<clean"
	cd obj
	rm -f *.obj *.exe
	cd ..

#rebuild: 
rebuild remake:
	echo "<<<<rebuild"
	make clean
	make target
	
#help: )
help:
	echo "<<<<help"
	egrep "^#" [Mm]akefile
	
#dumb:
dumb:
	echo $(OBJ)
	echo $(_OBJ)
	echo 