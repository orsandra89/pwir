CC = clang++
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

SRCS = Matrix.cpp test.cpp

OBJS = $(SRCS:.cpp=.o)

TESTTARGET = test.out

$(TESTTARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TESTTARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TESTTARGET)
