CC = clang++
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

CCMPI = mpic++
CCMPIFLAGS = -std=c++17 -Wunused-command-line-argument

SRCS = Matrix.cpp
TESTSRCS = test.cpp
SEQTESTSRCS = MatrixTest.cpp SequentialMatrixTest.cpp SequentialTest.cpp
MPTESTSRCS = MatrixTest.cpp MPMatrixTest.cpp MPTest.cpp
MPITESTSRCS = $(SRCS) test_mpi.cpp

OBJS = $(SRCS:.cpp=.o)
TESTOBJS = $(OBJS) $(TESTSRCS:.cpp=.o)
SEQTESTOBJS = $(OBJS) $(SEQTESTSRCS:.cpp=.o)
MPTESTOBJS = $(OBJS) $(MPTESTSRCS:.cpp=.o)

TESTTARGET = test.out
TESTMPITARGET = test_mpi.out
SEQTESTTARGET = sequential_test.out
MPTESTTARGET = mp_test.out

all: $(TESTTARGET) $(SEQTESTTARGET) $(MPTESTTARGET) $(TESTMPITARGET)

$(TESTTARGET): $(TESTOBJS)
	$(CC) $(CFLAGS) $(TESTOBJS) -o $(TESTTARGET)

$(TESTMPITARGET): $(MPITESTSRCS)
	$(CCMPI) $(CCMPIFLAGS) $(MPITESTSRCS) -o $(TESTMPITARGET)

$(SEQTESTTARGET): $(SEQTESTOBJS)
	$(CC) $(CFLAGS) $(SEQTESTOBJS) -o $(SEQTESTTARGET)

$(MPTESTTARGET): $(MPTESTOBJS)
	$(CC) $(CFLAGS) $(MPTESTOBJS) -o $(MPTESTTARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TESTOBJS) $(SEQTESTOBJS) $(MPTESTOBJS) $(TESTTARGET) $(TESTMPITARGET) $(SEQTESTTARGET) $(MPTESTTARGET)
