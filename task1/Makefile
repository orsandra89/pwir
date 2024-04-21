CC = clang++
CCMPI = mpic++
CCMPIFLAGS = -std=c++17 -Wunused-command-line-argument
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

# List of source files
SRCS = polynomial/Polynomial.cpp polynomial/QuadraticPolynomial.cpp
RECTANGLESRCS = IntegralTest.cpp rectangle/RectangleAlgorithm.cpp rectangle/test/RectangleAlgorithmTest.cpp RectangleTest.cpp
RECTANGLEMPSRCS = IntegralTest.cpp rectangle/RectangleAlgorithmOpenMP.cpp rectangle/test/RectangleAlgorithmOpenMPTest.cpp RectangleTestOpenMP.cpp
RECTANGLEMPISRCS = $(SRCS) IntegralTest.cpp rectangle/RectangleAlgorithmMPI.cpp rectangle/test/RectangleAlgorithmMPITest.cpp RectangleTestMPI.cpp
TRAPEZOIDSRCS = IntegralTest.cpp trapezoid/TrapezoidAlgorithm.cpp trapezoid/test/TrapezoidAlgorithmTest.cpp TrapezoidTest.cpp
TRAPEZOIDMPSRCS = IntegralTest.cpp trapezoid/TrapezoidAlgorithmOpenMP.cpp trapezoid/test/TrapezoidAlgorithmOpenMPTest.cpp TrapezoidTestOpenMP.cpp
TRAPEZOIDMPISRCS = $(SRCS) IntegralTest.cpp trapezoid/TrapezoidAlgorithmMPI.cpp trapezoid/test/TrapezoidAlgorithmMPITest.cpp TrapezoidTestMPI.cpp
SIMPSONSRCS = IntegralTest.cpp simpson/SimpsonAlgorithm.cpp simpson/test/SimpsonAlgorithmTest.cpp SimpsonTest.cpp
SIMPSONMPSRCS = IntegralTest.cpp simpson/SimpsonAlgorithmOpenMP.cpp simpson/test/SimpsonAlgorithmOpenMPTest.cpp SimpsonTestOpenMP.cpp
SIMPSONMPISRCS = $(SRCS) IntegralTest.cpp simpson/SimpsonAlgorithmMPI.cpp simpson/test/SimpsonAlgorithmMPITest.cpp SimpsonTestMPI.cpp
CLISRCS = simpson/SimpsonAlgorithm.cpp rectangle/RectangleAlgorithm.cpp trapezoid/TrapezoidAlgorithm.cpp rectangle/RectangleAlgorithmOpenMP.cpp trapezoid/TrapezoidAlgorithmOpenMP.cpp simpson/SimpsonAlgorithmOpenMP.cpp cli.cpp
# List of object files
OBJS = $(SRCS:.cpp=.o)
RECTANGLEOBJS = $(OBJS) $(RECTANGLESRCS:.cpp=.o)
RECTANGLEMPOBJS = $(OBJS) $(RECTANGLEMPSRCS:.cpp=.o)
TRAPEZOIDOBJS = $(OBJS) $(TRAPEZOIDSRCS:.cpp=.o)
TRAPEZOIDMPOBJS = $(OBJS) $(TRAPEZOIDMPSRCS:.cpp=.o)
SIMPSONOBJS = $(OBJS) $(SIMPSONSRCS:.cpp=.o)
SIMPSONMPOBJS = $(OBJS) $(SIMPSONMPSRCS:.cpp=.o)
CLIOBJS = $(OBJS) $(CLISRCS:.cpp=.o)

# Target executable
RECTANGLETARGET = rectangle_test.out
RECTANGLEMPTARGET = rectangle_mp_test.out
RECTANGLEMPITARGET = rectangle_mpi_test.out
TRAPEZOIDTARGET = trapezoid_test.out
TRAPEZOIDMPTARGET = trapezoid_mp_test.out
TRAPEZOIDMPITARGET = trapezoid_mpi_test.out
SIMPSONTARGET = simpson_test.out
SIMPSONMPTARGET = simpson_mp_test.out
SIMPSONMPITARGET = simpson_mpi_test.out
CLITARGET = cli.out

all: $(RECTANGLETARGET) $(RECTANGLEMPTARGET) $(RECTANGLEMPITARGET) $(TRAPEZOIDTARGET) $(TRAPEZOIDMPTARGET) $(TRAPEZOIDMPITARGET) $(SIMPSONTARGET) $(SIMPSONMPTARGET) $(SIMPSONMPITARGET) $(CLITARGET)

$(RECTANGLETARGET): $(RECTANGLEOBJS)
	$(CC) $(CFLAGS) $(RECTANGLEOBJS) -o $(RECTANGLETARGET)

$(RECTANGLEMPTARGET): $(RECTANGLEMPOBJS)
	$(CC) $(CFLAGS) $(RECTANGLEMPOBJS) -o $(RECTANGLEMPTARGET)

$(RECTANGLEMPITARGET): $(RECTANGLEMPISRCS)
	$(CCMPI) $(CCMPIFLAGS) $(RECTANGLEMPISRCS) -o $(RECTANGLEMPITARGET)

$(TRAPEZOIDTARGET): $(TRAPEZOIDOBJS)
	$(CC) $(CFLAGS) $(TRAPEZOIDOBJS) -o $(TRAPEZOIDTARGET)
	
$(TRAPEZOIDMPTARGET): $(TRAPEZOIDMPOBJS)
	$(CC) $(CFLAGS) $(TRAPEZOIDMPOBJS) -o $(TRAPEZOIDMPTARGET)

$(TRAPEZOIDMPITARGET): $(TRAPEZOIDMPISRCS)
	$(CCMPI) $(CCMPIFLAGS) $(TRAPEZOIDMPISRCS) -o $(TRAPEZOIDMPITARGET)

$(SIMPSONTARGET): $(SIMPSONOBJS)
	$(CC) $(CFLAGS) $(SIMPSONOBJS) -o $(SIMPSONTARGET)

$(SIMPSONMPTARGET): $(SIMPSONMPOBJS)
	$(CC) $(CFLAGS) $(SIMPSONMPOBJS) -o $(SIMPSONMPTARGET)

$(SIMPSONMPITARGET): $(SIMPSONMPISRCS)
	$(CCMPI) $(CCMPIFLAGS) $(SIMPSONMPISRCS) -o $(SIMPSONMPITARGET)

$(CLITARGET): $(CLIOBJS)
	$(CC) $(CFLAGS) $(CLIOBJS) -o $(CLITARGET)

# Rule for compiling source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(RECTANGLEOBJS) $(RECTANGLEMPOBJS) $(TRAPEZOIDOBJS) $(TRAPEZOIDMPOBJS) $(SIMPSONOBJS) $(SIMPSONMPOBJS) $(RECTANGLETARGET) $(RECTANGLEMPTARGET) $(RECTANGLEMPITARGET) $(TRAPEZOIDTARGET) $(TRAPEZOIDMPTARGET) $(TRAPEZOIDMPITARGET) $(SIMPSONTARGET) $(SIMPSONMPTARGET) $(SIMPSONMPITARGET) $(CLIOBJS) $(CLITARGET)