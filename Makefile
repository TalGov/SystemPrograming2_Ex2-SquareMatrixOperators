#talgov44@gmail.com

#Flags:
CXX = clang++
CXXFLAGS =  -Wextra -Werror -g -std=c++17 -Iinclude -Itests
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --error-exitcode=99

#Files:
TARGET_MAIN_EXE = main
TARGET_TEST_EXE = test

SRCS = src/SquareMat.cpp
MAIN_SRC = main.cpp
TEST_SRC = tests/test_SquareMat.cpp
SRC_DIR = src

all: $(TARGET_MAIN_EXE) $(TARGET_TEST_EXE) #Make all

#Make main
$(TARGET_MAIN_EXE): $(SRCS) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

#Make test
$(TARGET_TEST_EXE): $(SRCS) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

#Make valgrind
valgrind: $(TARGET_MAIN_EXE) $(TARGET_TEST_EXE)
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./$(TARGET_MAIN_EXE)
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./$(TARGET_TEST_EXE)

#Make clean
clean:
	rm -f $(OBJS) $(TARGET_MAIN_EXE) $(TARGET_TEST_EXE)


.PHONY: all clean valgrind