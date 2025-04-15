rarrayBASE?=/Users/patrickdeng/Cpp_Libraries/
rarrayINC=$(rarrayBASE)include
rarrayLIB=$(rarrayBASE)lib
brewBASE?=/opt/homebrew/
brewINC=$(brewBASE)include
brewLIB=$(brewBASE)lib

CXXFLAGS = -I$(rarrayINC) \
           -I$(brewINC) \
           -std=c++17 -O3 -Wall -g
LDFLAGS  = -L$(rarrayLIB) \
           -L$(brewLIB) -g

# Make the hydrogen program
all: hydrogen

hydrogen.o: hydrogen.cpp eigenval.h output.h init.h
	$(CXX) $(CXXFLAGS) -c -o $@ hydrogen.cpp

eigenval.o: eigenval.cpp eigenval.h
	$(CXX) $(CXXFLAGS) -c -o $@ eigenval.cpp

output.o: output.cpp output.h
	$(CXX) $(CXXFLAGS) -c -o $@ output.cpp

init.o: init.cpp init.h
	$(CXX) $(CXXFLAGS) -c -o $@ init.cpp

hydrogen: hydrogen.o eigenval.o output.o init.o 
	$(CXX) $(LDFLAGS) -o $@ $^

# The output_c2 test program
output_c2.o: output_c2.cpp output.h
	$(CXX) $(CXXFLAGS) -c -o $@ output_c2.cpp
output_c2: output_c2.o output.o 
	$(CXX) $(LDFLAGS) -o $@ $^ -lCatch2Main -lCatch2 
run_output_c2: output_c2			# Run the output_c2 test program
	./output_c2 -s

# The init_test program
init_test.o: init_test.cpp init.h
	$(CXX) $(CXXFLAGS) -c -o $@ init_test.cpp
init_test: init_test.o init.o		# Link the test executable for the init module
	$(CXX) $(LDFLAGS) -o $@ $^ -lCatch2Main -lCatch2
run_init_test: init_test
	./init_test -s

# The eigenval_test program
eig_test.o: eig_test.cpp eigenval.h
	$(CXX) $(CXXFLAGS) -c -o $@ eig_test.cpp
eig_test: eig_test.o eigenval.o		# Link the test executable for the eigenval module
	$(CXX) $(LDFLAGS) -o $@ $^ -lCatch2Main -lCatch2
run_eig_test: eig_test
	./eig_test -s

# The integrated_test program
integrated_test.o: integrated_test.cpp eigenval.h init.h output.h
	$(CXX) $(CXXFLAGS) -c -o $@ integrated_test.cpp
integrated_test: integrated_test.o eigenval.o	init.o output.o	# Link the test executable for all the modules
	$(CXX) $(LDFLAGS) -o $@ $^ -lCatch2Main -lCatch2
run_integrated_test: integrated_test
	./integrated_test -s

run_all: run_output_c2 run_init_test run_eig_test run_integrated_test

clean:
	$(RM) hydrogen.o eigenval.o output.o init.o output_c2.o hydrogen output_c2 \
	       init_test.o init_test eig_test.o eig_test integrated_test.o integrated_test

.PHONY: all clean run_output_c2 run_init_test run_eig_test run_integrated_test run_all
