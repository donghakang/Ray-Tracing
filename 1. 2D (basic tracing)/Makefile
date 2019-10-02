CXX = g++
CXXFLAGS = -c -Wall -g -std=c++11

all: kangx637

kangx637: RayTracing.o File.o vectorMath.o
	$(CXX) RayTracing.o File.o vectorMath.o -o kangx637

RayTracing.o: RayTracing.cpp
	$(CXX) $(CXXFLAGS) RayTracing.cpp

vectorMath.o: vectorMath.cpp
	$(CXX) $(CXXFLAGS) vectorMath.cpp

File.o: File.cpp
	$(CXX) $(CXXFLAGS) vectorMath.cpp File.cpp

clean:
	rm -rf *o kangx637
