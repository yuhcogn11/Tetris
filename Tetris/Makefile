CXX = g++
CXXFLAGS = -I C:/SDL/x86_64-w64-mingw32/include
LDFLAGS = -L C:/SDL/x86_64-w64-mingw32/lib -lSDL3

SRC = $(wildcard src/*.cpp)
OUT = main.exe

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

clean:
	del $(OUT)
