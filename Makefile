CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I . -g 
SRC = test.cpp
LIBS = -ldl
OUT = test 

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(LIBS) -o $(OUT)

clean:
	rm -f $(OUT)
