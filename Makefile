CXX	:= g++-4.4
CXXFLAGS := 
OBJECTS := cuttingStock.o 

all: cuttingStock

cuttingStock: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o cuttingStock
	#rm -rf *.o *.out semantic.cache *.gch *~ *.tex.backup

cuttingStock.o: cuttingStock.cpp
	$(CXX) $(CXXFLAGS) -c cuttingStock.cpp		

clean:
	rm -rf cuttingStock *.o *.out semantic.cache *.gch *~ *.tex.backup
