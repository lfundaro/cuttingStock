CXX	:= g++-4.4
CXXFLAGS := 
OBJECTS := cuttingStock.o FFD.o utilities.o

all: cuttingStock

cuttingStock: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o cuttingStock
	#rm -rf *.o *.out semantic.cache *.gch *~ *.tex.backup

cuttingStock.o: cuttingStock.cpp
	$(CXX) $(CXXFLAGS) -c cuttingStock.cpp		

FFD.o: FFD.cpp
	$(CXX) $(CXXFLAGS) -c FFD.cpp		

utilities.o: utilities.cpp
	$(CXX) $(CXXFLAGS) -c utilities.cpp		

clean:
	rm -rf cuttingStock *.o *.out semantic.cache *.gch *~ *.tex.backup
