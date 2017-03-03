#simple makefile
CXX=g++
FLAGS=-std=c++11 -Wall -DENABLE_DEBUG

optim-proj: optim-proj.cpp
	$(CXX) $(FLAGS) optim-proj.cpp -o optim-proj

clean:
	rm -rf *.o optim-proj

