# Makefile
CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall -g -I.
LDFLAGS=-g
RM=rm -f

all: gameof1d

gameof1d: gameof1d_modular.o init.o cell_alive.o cell_print.o cell_update.o
	$(CXX) $(LDFLAGS) -o gameof1d_modular gameof1d_modular.o init.o cell_alive.o cell_print.o cell_update.o

gameof1d_modular.o: gameof1d_modular.cpp init.o cell_alive.h cell_print.h cell_update.h
	$(CXX) -c $(CXXFLAGS) -o gameof1d_modular.o gameof1d_modular.cpp

init.o: init.cpp init.h
	$(CXX) -c $(CXXFLAGS) -o init.o init.cpp

cell_alive.o: cell_alive.cpp cell_alive.h
	$(CXX) -c $(CXXFLAGS) -o cell_alive.o cell_alive.cpp

cell_print.o: cell_print.cpp cell_print.h
	$(CXX) -c $(CXXFLAGS) -o cell_print.o cell_print.cpp

cell_update.o: cell_update.cpp cell_update.h
	$(CXX) -c $(CXXFLAGS) -o cell_update.o cell_update.cpp

clean:
	$(RM) gameof1d_modular.o init.o cell_alive.o cell_print.o cell_update.o
.PHONY: all clean