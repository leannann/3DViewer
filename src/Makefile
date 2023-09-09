CC=gcc
FLAGS= -Wall -Werror -Wextra -std=c++17 -lstdc++ -lgtest
GCOVFLAGS= -fprofile-arcs -ftest-coverage

all: gcov_report

install: clean
	@mkdir -p build
	@cd build/ && qmake ../Viewer/Viewer.pro && make
	@./build/Viewer.app/Contents/MacOS/Viewer

open_app:
	@./build/Viewer.app/Contents/MacOS/Viewer

uninstall:
	@rm -rf build

tests:
	@$(CC) $(FLAGS) $(GCOVFLAGS)  -o test ./Viewer/model.cc ./Viewer/affine.cc ./unit/googletests.cc
	@leaks -atExit -- ./test

gcov_report: tests
	@geninfo --ignore-errors mismatch  . --output-file test.info
	@genhtml  -o report test.info
	@open report/index.html

dvi:
	@doxygen Doxyfile && mv html documentation
	@open documentation/index.html

dist: clean
	@mkdir 3DViewer
	@cp Viewer/*.cc Viewer/*.h Viewer/*.ui Viewer/*.pro Viewer/*.user Makefile Doxyfile 3DViewer
	@tar -cvzf dist.tar.gz 3DViewer
	@-rm -rf 3DViewer

clean: 
	@rm -rf test *.a *.gch *.gcno *.gcna *.gcda *.info *.dSYM test_html .qmake.stash unit_tests report documentation latex *.gz
