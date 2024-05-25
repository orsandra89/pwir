CC = clang++
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

CCMPI = mpic++
CCMPIFLAGS = -std=c++17 -Wunused-command-line-argument

SRCS = Matrix.cpp
TESTSRCS = test.cpp
TIMECALCULATIONSRCS = calculate_time.cpp
MPITESTSRCS = $(SRCS) test_mpi.cpp

OBJS = $(SRCS:.cpp=.o)
TESTOBJS = $(OBJS) $(TESTSRCS:.cpp=.o)
TIMECALCULATIONOBJS = $(OBJS) $(TIMECALCULATIONSRCS:.cpp=.o)

TESTTARGET = test.out
TIMECALCULATIONTARGET = calculate_time.out
TESTMPITARGET = test_mpi.out

all: $(TESTTARGET) $(TIMECALCULATIONTARGET) $(TESTMPITARGET)

$(TESTTARGET): $(TESTOBJS)
	$(CC) $(CFLAGS) $(TESTOBJS) -o $(TESTTARGET)

$(TIMECALCULATIONTARGET): $(TIMECALCULATIONOBJS)
	$(CC) $(CFLAGS) $(TIMECALCULATIONOBJS) -o $(TIMECALCULATIONTARGET)

$(TESTMPITARGET): $(MPITESTSRCS)
	$(CCMPI) $(CCMPIFLAGS) $(MPITESTSRCS) -o $(TESTMPITARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TESTOBJS) $(TESTTARGET) $(TESTMPITARGET) $(TIMECALCULATIONOBJS) $(TIMECALCULATIONTARGET)
