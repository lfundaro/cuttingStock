CXX	:= g++-4.4
CXXFLAGS := -g
OBJECTS := cuttingStock.o FFD.o utilities.o localSearchBB.o shiftSpace.o Perturb.o ILS.o

all: cuttingStock

cuttingStock: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o cuttingStock
	#rm -rf *.o *.out semantic.cache *.gch *~ *.tex.backup

cuttingStock.o: cuttingStock.cpp
	$(CXX) $(CXXFLAGS) -c cuttingStock.cpp		

FFD.o: FFD.cpp
	$(CXX) $(CXXFLAGS) -c FFD.cpp		

shiftSpace.o: shiftSpace.cpp
	$(CXX) $(CXXFLAGS) -c shiftSpace.cpp

localSearchBB.o: localSearchBB.cpp
	$(CXX) $(CXXFLAGS) -c localSearchBB.cpp		

Perturb.o: Perturb.cpp
	$(CXX) $(CXXFLAGS) -c Perturb.cpp

ILS.o: ILS.cpp
	$(CXX) $(CXXFLAGS) -c ILS.cpp

utilities.o: utilities.cpp
	$(CXX) $(CXXFLAGS) -c utilities.cpp		

clean:
	rm -rf cuttingStock *.o *.out semantic.cache *.gch *~ *.tex.backup test
