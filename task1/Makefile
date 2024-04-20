CC = g++
CFLAGS = -std=c++17

# List of source files
SRCS = Polynomial.cpp QuadraticPolynomial.cpp RectangleAlgorithm.cpp Test.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = output.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Rule for compiling source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)