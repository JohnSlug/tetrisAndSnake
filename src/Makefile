.PHONY: all install_cli install_desktop uninstall clean clean_dvi dvi dist test_tetris test_snake gcov_report_tetris gcov_report_snake run_cli run_desktop style_check style style-fix

SHELL := /bin/bash

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17
LDFLAGS = -lncurses

DIR = ./gamedir/
DIST_DIR = archive

BACKEND_TETRIS = $(wildcard brick_game/tetris/*.c) 
BACKEND_SNAKE = $(wildcard brick_game/snake/*.cc)

FRONTEND_TETRIS := $(wildcard gui/cli/tetris.c) $(wildcard gui/cli/interface_tetris.c)
FRONTEND_SNAKE := $(wildcard gui/cli/interface_snake.cc) $(wildcard gui/cli/snake.cc)

EXECUTABLE = brick_game_exe
DESKTOP_EXECUTABLE = view

TETRIS_OBJ = $(BACKEND_TETRIS:.c=.o) $(FRONTEND_TETRIS:.c=.o)
SNAKE_OBJ = $(BACKEND_SNAKE:.cc=.o) $(FRONTEND_SNAKE:.cc=.o)

MAIN_SRC = gui/cli/main.cc
MAIN_OBJ = $(MAIN_SRC:.cc=.o)

TEST_FILES_TETRIS = tests/test_tetris.c
TEST_FILES_SNAKE = tests/test_snake.cc

CMAKE_PATH = ./gui/desktop/view
BUILDPATH = $(CMAKE_PATH)/build

UNAME = $(shell uname -s)

ifeq ($(UNAME), Linux)
    LIBS = -lcheck -lgtest -lm -lpthread -lrt -lsubunit
else
    LIBS = -lcheck -lgtest -lm -lpthread
endif

all: clean install_desktop install_cli run_desktop

install_cli: $(TETRIS_OBJ) $(SNAKE_OBJ) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(EXECUTABLE) $(LDFLAGS)

run_cli: $(EXECUTABLE)
	./$(EXECUTABLE)

install_desktop: 
	mkdir -p $(BUILDPATH) && \
	cd $(BUILDPATH) && \
	cmake .. && \
	make

run_desktop: 
	open $(BUILDPATH)/view.app

uninstall:
	@rm -f $(EXECUTABLE)
	rm -rf $(BUILDPATH)
	@echo "Uninstallation is finished"

dvi: clean_dvi
	cd ..
	doxygen Doxyfile
	open html/index.html

dist: clean
	mkdir -p $(DIST_DIR)
	cp -rf brick_game/ $(DIST_DIR)/brick_game/
	cp -rf gui/ $(DIST_DIR)/gui/
	cp -rf Makefile $(DIST_DIR)/
	tar -czvf $(DIST_DIR).tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

test_tetris: clean
	$(CC) $(CFLAGS) $(BACKEND_TETRIS) $(TEST_FILES_TETRIS) -o test_tetris $(LDFLAGS) $(LIBS)
	./test_tetris

test_snake: clean
	$(CXX) $(CXXFLAGS) brick_game/snake/snake_logic.cc $(TEST_FILES_SNAKE) -lgtest -lgtest_main -pthread -o test_snake $(LDFLAGS) $(LIBS)
	./test_snake

gcov_report_tetris: clean
	@mkdir -p report
	$(CC) -g $(BACKEND_TETRIS) $(TEST_FILES_TETRIS) $(LIBS) --coverage -o ./gcov_report_tetris.out
	./gcov_report_tetris.out
	lcov --ignore-errors inconsistent,empty --no-external -t "test" -o test.info -c -d .
	genhtml -o report/ test.info
	open report/index.html

gcov_report_snake: clean
	@mkdir -p report
	$(CXX) -g -std=c++17 brick_game/snake/snake_logic.cc $(TEST_FILES_SNAKE) -lgtest -lgtest_main -pthread --coverage -o ./gcov_report_snake.out
	./gcov_report_snake.out
	lcov --ignore-errors inconsistent,empty --no-external -t "test" -o test.info -c -d .
	genhtml -o report/ test.info
	open report/index.html


style_check:
	clang-format -style=Google -n ./{brick_game/tetris,gui/cli,tests/}/*.{h,c}
	clang-format -style=Google -n ./{brick_game/snake,gui/cli,tests/}/*.{h,cc}
	clang-format -style=Google -n ./gui/desktop/view/*.cpp

style:
	clang-format -style=Google -i ./{brick_game/tetris,gui/cli,tests/}/*.{h,c}
	clang-format -style=Google -i ./{brick_game/snake,gui/cli,tests/}/*.{h,cc}
	clang-format -style=Google -i ./gui/desktop/view/*.cpp

clean_dvi:
	rm -rf materials/docs/

clean:
	rm -f *.o $(EXECUTABLE) $(DESKTOP_EXECUTABLE)
	rm -f test_snake test_tetris
	@find .. -type f -name "*.a" -exec rm {} \;
	@find .. -type f -name "*.out" -exec rm {} \;
	@find ./gui/desktop/view -type f -name "Makefile" -exec rm -f {} \;
	@find .. -type f -name "cmake_install.cmake" -exec rm {} \;
	@find .. -type f -name "CMakeCache.txt" -exec rm {} \;
	@if [ -d "../src/gcov_report_snake.out.dSYM" ]; then rm -rf ../src/gcov_report_snake.out.dSYM; fi
	@if [ -d "../src/gcov_report_tetris.out.dSYM" ]; then rm -rf ../src/gcov_report_tetris.out.dSYM; fi
	@find .. -type f -name "*.gcda" -exec rm {} \;
	@find .. -type f -name "*.gcno" -exec rm {} \;
	@find .. -type f -name "*.info" -exec rm {} \;
	@find .. -type f -name "*.o" -exec rm {} \;
	@rm -rf report
	@rm -rf ${DIST_DIR}
	@rm -rf ${DIST_DIR}.tar.gz