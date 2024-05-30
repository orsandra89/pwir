CC = clang++
CFLAGS = -std=c++17 -Wunused-command-line-argument -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

CCMPI = mpic++
CCMPIFLAGS = -std=c++17 -Wunused-command-line-argument

SRCS = BPM.cpp Line.cpp
TESTSRCS = test.cpp
# TIMECALCULATIONSRCS = calculate_time.cpp

OBJS = $(SRCS:.cpp=.o)
TESTOBJS = $(OBJS) $(TESTSRCS:.cpp=.o)
# TIMECALCULATIONOBJS = $(OBJS) $(TIMECALCULATIONSRCS:.cpp=.o)

TESTTARGET = test.out
# TIMECALCULATIONTARGET = calculate_time.out

all: $(TESTTARGET)

$(TESTTARGET): $(TESTOBJS)
	$(CC) $(CFLAGS) $(TESTOBJS) -o $(TESTTARGET)

# $(TIMECALCULATIONTARGET): $(TIMECALCULATIONOBJS)
# 	$(CC) $(CFLAGS) $(TIMECALCULATIONOBJS) -o $(TIMECALCULATIONTARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TESTOBJS) $(TESTTARGET)
