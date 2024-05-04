CC = clang++
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

SRCS = Matrix.cpp
TESTSRCS = test.cpp
SEQTESTSRCS = MatrixTest.cpp SequentialMatrixTest.cpp SequentialTest.cpp
MPTESTSRCS = MatrixTest.cpp MPMatrixTest.cpp MPTest.cpp

OBJS = $(SRCS:.cpp=.o)
TESTOBJS = $(OBJS) $(TESTSRCS:.cpp=.o)
SEQTESTOBJS = $(OBJS) $(SEQTESTSRCS:.cpp=.o)
MPTESTOBJS = $(OBJS) $(MPTESTSRCS:.cpp=.o)

TESTTARGET = test.out
SEQTESTTARGET = sequential_test.out
MPTESTTARGET = mp_test.out

all: $(TESTTARGET) $(SEQTESTTARGET) $(MPTESTTARGET)

$(TESTTARGET): $(TESTOBJS)
	$(CC) $(CFLAGS) $(TESTOBJS) -o $(TESTTARGET)

$(SEQTESTTARGET): $(SEQTESTOBJS)
	$(CC) $(CFLAGS) $(SEQTESTOBJS) -o $(SEQTESTTARGET)

$(MPTESTTARGET): $(MPTESTOBJS)
	$(CC) $(CFLAGS) $(MPTESTOBJS) -o $(MPTESTTARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TESTOBJS) $(SEQTESTOBJS) $(MPTESTOBJS) $(TESTTARGET) $(SEQTESTTARGET) $(MPTESTTARGET)
