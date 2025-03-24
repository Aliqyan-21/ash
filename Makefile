CXX = g++
CXXFLAGS = -Wall -std=c++20

ash : main.cpp
	${CXX} ${CXXFLAGS} main.cpp -o ash

run :
	@./ash

clean :
	rm ash
