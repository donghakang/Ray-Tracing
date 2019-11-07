CXX = g++
CXXFLAGS = -c -Wall -g -std=c++11

all: kangx637

kangx637: RayTracing.o File.o VectorMath.o Shading.o SpecialTexture.o
	$(CXX) -o kangx637 RayTracing.o File.o VectorMath.o Shading.o SpecialTexture.o

RayTracing.o: RayTracing.cpp
	$(CXX) $(CXXFLAGS) RayTracing.cpp

VectorMath.o: VectorMath.cpp
	$(CXX) $(CXXFLAGS) VectorMath.cpp

Shading.o: Shading.cpp
	$(CXX) $(CXXFLAGS) Shading.cpp

Texture.o: SpecialTexture.cpp
	$(CXX) $(CXXFLAGS) SpecialTexture.cpp

File.o: File.cpp
	$(CXX) $(CXXFLAGS) VectorMath.cpp File.cpp

clean:
	rm -rf *o kangx637
