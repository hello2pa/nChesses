all: nChesses

nChesses: ./src/main.cpp
	g++ -o nChesses -I./include -lncurses ./src/main.cpp
