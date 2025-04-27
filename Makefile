#talgov44@gmail.com

CXX = clang++
CXXFLAGS = -Wall -g -std=c++11 -lstdc++
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --error-exitcode=99

SRCS =
MAIN_SRC = main.cpp
TEST_SRC = TestMain.cpp Test.cpp


all: main test

main: main.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(SRCS) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

valgrind: main test
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./main
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test

clean:
	rm -f $(OBJS) TestMain.o Test.o main test


.PHONY: all clean valgrind