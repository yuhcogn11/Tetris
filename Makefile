CXX = g++

CXXFLAGS = -I C:/Users/ad/Desktop/Tetris/SDL/include \
           -I C:/Users/ad/Desktop/Tetris/SDL/include/SDL3_image \
           -I C:/Users/ad/Desktop/Tetris/SDL/include/SDL3_ttf \
           -I C:/Users/ad/Desktop/DSA/SDL3_mixer-devel/SDL3_mixer-3.2.0/x86_64-w64-mingw32/include

LDFLAGS = -L C:/Users/ad/Desktop/Tetris/SDL/lib \
          -L C:/Users/ad/Desktop/DSA/SDL3_mixer-devel/SDL3_mixer-3.2.0/x86_64-w64-mingw32/lib \
          -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer

SRC = $(wildcard src/*.cpp)
OUT = main.exe

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

clean:
	del $(OUT)