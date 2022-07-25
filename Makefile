all: compile run
compile:
	g++ main.cpp -o main -I ./deps/include/ -L ./deps/lib/ -lraylib -lGL -lm -lpthread -ldl -lrt
run:
	./main
