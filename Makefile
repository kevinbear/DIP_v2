.PHONY:all clean
CC=g++
CFLAG= -g -Wall -lpthread -O3 -o 
OBG= src/bmp.cpp
MAIN= src/main.cpp
all:
	$(CC) $(OBG) $(MAIN) -c 
	$(CC) bmp.o main.o src/bmp.h $(CFLAG) DIP
clean:
	rm -rf *.o DIP *.*~ *~
	rm -rf Gussian_LPT.bmp Error_diffusion.bmp Sobel_HPT.bmp

