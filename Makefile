all:
	clang++ -o sdl2_opengl_test.out -std=c++11 -Wextra -Wall -Wpedantic main.cpp `pkg-config sdl2 --cflags --libs` -l GL -l GLU -l glut
run:
	./sdl2_opengl_test.out

