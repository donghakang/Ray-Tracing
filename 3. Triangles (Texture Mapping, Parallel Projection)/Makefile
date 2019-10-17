CXX = g++
CXXFLAGS = -c -Wall -g -std=c++11

all: kangx637

kangx637: RayTracing.o File.o vectorMath.o Shading.o Texture.o
	$(CXX) -o kangx637 RayTracing.o File.o vectorMath.o Shading.o Texture.o

RayTracing.o: RayTracing.cpp
	$(CXX) $(CXXFLAGS) RayTracing.cpp

vectorMath.o: vectorMath.cpp
	$(CXX) $(CXXFLAGS) vectorMath.cpp

Shading.o: Shading.cpp
	$(CXX) $(CXXFLAGS) Shading.cpp

Texture.o: Texture.cpp
	$(CXX) $(CXXFLAGS) Texture.cpp

File.o: File.cpp
	$(CXX) $(CXXFLAGS) vectorMath.cpp File.cpp

clean:
	rm -rf *o kangx637
