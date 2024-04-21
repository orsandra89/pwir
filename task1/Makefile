CC = clang++
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

# List of source files
SRCS = Polynomial.cpp QuadraticPolynomial.cpp
RECTANGLESRCS = RectangleAlgorithm.cpp RectangleTest.cpp
RECTANGLEMPSRCS = RectangleAlgorithmOpenMP.cpp RectangleTestOpenMP.cpp
TRAPEZOIDSRCS = TrapezoidAlgorithm.cpp TrapezoidTest.cpp
TRAPEZOIDMPSRCS = TrapezoidAlgorithmOpenMP.cpp TrapezoidTestOpenMP.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)
RECTANGLEOBJS = $(OBJS) $(RECTANGLESRCS:.cpp=.o)
RECTANGLEMPOBJS = $(OBJS) $(RECTANGLEMPSRCS:.cpp=.o)
TRAPEZOIDOBJS = $(OBJS) $(TRAPEZOIDSRCS:.cpp=.o)
TRAPEZOIDMPOBJS = $(OBJS) $(TRAPEZOIDMPSRCS:.cpp=.o)

# Target executable
RECTANGLETARGET = rectangle_test.out
RECTANGLEMPTARGET = rectangle_mp_test.out
TRAPEZOIDTARGET = trapezoid_test.out
TRAPEZOIDMPTARGET = trapezoid_mp_test.out

all: $(RECTANGLETARGET) $(RECTANGLEMPTARGET) $(TRAPEZOIDTARGET) $(TRAPEZOIDMPTARGET)

$(RECTANGLETARGET): $(RECTANGLEOBJS)
	$(CC) $(CFLAGS) $(RECTANGLEOBJS) -o $(RECTANGLETARGET)

$(RECTANGLEMPTARGET): $(RECTANGLEMPOBJS)
	$(CC) $(CFLAGS) $(RECTANGLEMPOBJS) -o $(RECTANGLEMPTARGET)

$(TRAPEZOIDTARGET): $(TRAPEZOIDOBJS)
	$(CC) $(CFLAGS) $(TRAPEZOIDOBJS) -o $(TRAPEZOIDTARGET)
	
$(TRAPEZOIDMPTARGET): $(TRAPEZOIDMPOBJS)
	$(CC) $(CFLAGS) $(TRAPEZOIDMPOBJS) -o $(TRAPEZOIDMPTARGET)

# Rule for compiling source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(RECTANGLEOBJS) $(RECTANGLEMPOBJS) $(TRAPEZOIDOBJS) $(TRAPEZOIDMPOBJS) $(RECTANGLETARGET) $(RECTANGLEMPTARGET) $(TRAPEZOIDTARGET) $(TRAPEZOIDMPTARGET)