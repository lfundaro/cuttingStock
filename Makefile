CXX	:= g++-4.4
CXXFLAGS := -g
OBJECTS := cuttingStock.o FFD.o utilities.o localSearchBB.o shiftSpace.o Perturb.o ILS.o rand_rst.o GRASP.o

all: cuttingStock

cuttingStock: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o cuttingStock
	#rm -rf *.o *.out semantic.cache *.gch *~ *.tex.backup

cuttingStock.o: cuttingStock.cpp
	$(CXX) $(CXXFLAGS) -c cuttingStock.cpp		

FFD.o: FFD.cpp FFD.h
	$(CXX) $(CXXFLAGS) -c FFD.cpp		

shiftSpace.o: shiftSpace.cpp shiftSpace.h
	$(CXX) $(CXXFLAGS) -c shiftSpace.cpp

localSearchBB.o: localSearchBB.cpp localSearchBB.h
	$(CXX) $(CXXFLAGS) -c localSearchBB.cpp		

Perturb.o: Perturb.cpp Perturb.h
	$(CXX) $(CXXFLAGS) -c Perturb.cpp

ILS.o: ILS.cpp ILS.h
	$(CXX) $(CXXFLAGS) -c ILS.cpp

GRASP.o: GRASP.cpp GRASP.h
	$(CXX) $(CXXFLAGS) -c GRASP.cpp

rand_rst.o: rand_rst.cpp rand_rst.h
	$(CXX) $(CXXFLAGS) -c rand_rst.cpp

utilities.o: utilities.cpp
	$(CXX) $(CXXFLAGS) -c utilities.cpp		

Solution.o: Solution.cpp
	$(CXX) $(CXXFLAGS) -c Solution.cpp	

clean:
	rm -rf cuttingStock *.o *.out semantic.cache *.gch *~ *.tex.backup test
