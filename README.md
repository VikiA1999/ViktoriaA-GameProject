# Space Shooter Game

## How to compile
Windows : gcc miain.c -o game -lraylib -lopeng123 -lgdi32 -lwinmm

Linux : gcc main.c -o game -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

Mac : gcc main.c -o game -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

## How to run
Windows : game.exe

Linux : ./game

Mac : ./game

## Notes
Make sure that all the texture files are in the same folder as the game.
