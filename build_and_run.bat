g++ -std=c++17 -O3 -flto -march=native -mtune=native main.cpp .\core\*.cpp -ISDL2\include -LSDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o gb-emulator
gb-emulator.exe gb_bios.bin Tetris.gb