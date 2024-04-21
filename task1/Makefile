CC = clang++
CCMPI = mpicc
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

# List of source files
SRCS = polynomial/Polynomial.cpp polynomial/QuadraticPolynomial.cpp
RECTANGLESRCS = IntegralTest.cpp rectangle/RectangleAlgorithm.cpp rectangle/test/RectangleAlgorithmTest.cpp RectangleTest.cpp
RECTANGLEMPSRCS = IntegralTest.cpp rectangle/RectangleAlgorithmOpenMP.cpp rectangle/test/RectangleAlgorithmOpenMPTest.cpp RectangleTestOpenMP.cpp
TRAPEZOIDSRCS = IntegralTest.cpp trapezoid/TrapezoidAlgorithm.cpp trapezoid/test/TrapezoidAlgorithmTest.cpp TrapezoidTest.cpp
TRAPEZOIDMPSRCS = IntegralTest.cpp trapezoid/TrapezoidAlgorithmOpenMP.cpp trapezoid/test/TrapezoidAlgorithmOpenMPTest.cpp TrapezoidTestOpenMP.cpp
SIMPSONSRCS = IntegralTest.cpp simpson/SimpsonAlgorithm.cpp simpson/test/SimpsonAlgorithmTest.cpp SimpsonTest.cpp
SIMPSONMPSRCS = IntegralTest.cpp simpson/SimpsonAlgorithmOpenMP.cpp simpson/test/SimpsonAlgorithmOpenMPTest.cpp SimpsonTestOpenMP.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)
RECTANGLEOBJS = $(OBJS) $(RECTANGLESRCS:.cpp=.o)
RECTANGLEMPOBJS = $(OBJS) $(RECTANGLEMPSRCS:.cpp=.o)
TRAPEZOIDOBJS = $(OBJS) $(TRAPEZOIDSRCS:.cpp=.o)
TRAPEZOIDMPOBJS = $(OBJS) $(TRAPEZOIDMPSRCS:.cpp=.o)
SIMPSONOBJS = $(OBJS) $(SIMPSONSRCS:.cpp=.o)
SIMPSONMPOBJS = $(OBJS) $(SIMPSONMPSRCS:.cpp=.o)

# Target executable
RECTANGLETARGET = rectangle_test.out
RECTANGLEMPTARGET = rectangle_mp_test.out
TRAPEZOIDTARGET = trapezoid_test.out
TRAPEZOIDMPTARGET = trapezoid_mp_test.out
SIMPSONTARGET = simpson_test.out
SIMPSONMPTARGET = simpson_mp_test.out

all: $(RECTANGLETARGET) $(RECTANGLEMPTARGET) $(TRAPEZOIDTARGET) $(TRAPEZOIDMPTARGET) $(SIMPSONTARGET) $(SIMPSONMPTARGET)

$(RECTANGLETARGET): $(RECTANGLEOBJS)
	$(CC) $(CFLAGS) $(RECTANGLEOBJS) -o $(RECTANGLETARGET)

$(RECTANGLEMPTARGET): $(RECTANGLEMPOBJS)
	$(CC) $(CFLAGS) $(RECTANGLEMPOBJS) -o $(RECTANGLEMPTARGET)

$(TRAPEZOIDTARGET): $(TRAPEZOIDOBJS)
	$(CC) $(CFLAGS) $(TRAPEZOIDOBJS) -o $(TRAPEZOIDTARGET)
	
$(TRAPEZOIDMPTARGET): $(TRAPEZOIDMPOBJS)
	$(CC) $(CFLAGS) $(TRAPEZOIDMPOBJS) -o $(TRAPEZOIDMPTARGET)

$(SIMPSONTARGET): $(SIMPSONOBJS)
	$(CC) $(CFLAGS) $(SIMPSONOBJS) -o $(SIMPSONTARGET)

$(SIMPSONMPTARGET): $(SIMPSONMPOBJS)
	$(CC) $(CFLAGS) $(SIMPSONMPOBJS) -o $(SIMPSONMPTARGET)

# Rule for compiling source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(RECTANGLEOBJS) $(RECTANGLEMPOBJS) $(TRAPEZOIDOBJS) $(TRAPEZOIDMPOBJS) $(SIMPSONOBJS) $(SIMPSONMPOBJS) $(RECTANGLETARGET) $(RECTANGLEMPTARGET) $(TRAPEZOIDTARGET) $(TRAPEZOIDMPTARGET) $(SIMPSONTARGET) $(SIMPSONMPTARGET)