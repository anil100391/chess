all: debug

debug:
	g++ -g -Wall board.cpp main.cpp -o chess

release:
	g++ -O2 board.cpp main.cpp -o chess
