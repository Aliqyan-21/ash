CXX = g++
CXXFLAGS = -Wall -std=c++20

ash : ash_execvp.cpp ash.cpp main.cpp
	${CXX} ${CXXFLAGS} ash_execvp.cpp ash.cpp main.cpp -o ash

run :
	@./ash

clean :
	rm ash
