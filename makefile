CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -O2 -lm

all: main

main: main.cpp
	$(CC) $(CFLAGS) $< -o $@