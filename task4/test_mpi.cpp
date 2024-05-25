#include <iostream>
#include <vector>
#include <map>
#include <mpi.h>
#include "Matrix.cpp"

using namespace std;

#define ROW_START_TAG 0    //tag for communicating the start row of the workload for a worker
#define ROW_END_TAG 1      //tag for communicating the end row of the workload for a worker
#define A_COLS_TAG 2       //tag for communicating the address of the data to be worked on to worker
#define A_ROWS_TAG 3       //tag for communicating the address of the calculated data to master
#define LOCAL_TIME_TAG 4   //tag for communicating the address of the local matrix calculation time to master
#define A_TOTAL_ROWS_TAG 5
#define C_ROWS_TAG 6

double start_time, end_time;
double localTimeSaver;

std::vector<double> flatten(const std::vector<std::vector<double>>& matrix) {
    std::vector<double> flattened;
    for (const auto& row : matrix) {
        flattened.insert(flattened.end(), row.begin(), row.end());
    }
    return flattened;
}

bool stringToBool(const string& str) {
    static const map<string, bool> trueStringMap{
        {"true", true}, {"1", true}, {"yes", true}, {"y", true},
    };

    static const map<string, bool> falseStringMap{
        {"false", false}, {"0", false}, {"no", false}, {"n", false},
    };

    // Convert the input string to lowercase for case-insensitive comparison
    string lowerStr;
    for (char ch : str) {
        lowerStr += tolower(ch);
    }

    // Check if the string exists in the true or false mapping
    auto trueIt = trueStringMap.find(lowerStr);
    if (trueIt != trueStringMap.end()) {
        return trueIt->second;
    }

    auto falseIt = falseStringMap.find(lowerStr);
    if (falseIt != falseStringMap.end()) {
        return falseIt->second;
    }

    // If the string does not match any recognized boolean representation, return false by default
    return false;
}

int main(int argc, char *argv[]) {

    // Instantiate global variables used in the parallelization
    int rank;                  // mpi: process id number
    int nProcesses;            // mpi: number of total processess 
    MPI_Status status;         // mpi: store status of a MPI_Recv
    MPI_Request request;       // mpi: capture request of a MPI_Isend
    int rowStart, rowEnd;      // which rows of A that are calculated by the worker process
    int granularity; 	   // granularity of parallelization (# of rows per processor) 

    Matrix A_MATRIX = Matrix(0,0,0.0);

    string resultPath;
    bool complete;
    
    int numberOfRowsA;  
    int numberOfColsA;   

    vector<double> A;

    MPI_Init(&argc, &argv);		                  /* initialize MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);	      /* store the rank */
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses); /* store the number of processes */

    if (rank == 0) {  /* Master initializes work*/
        if (argc == 4) {
            string inputMatrix = argv[1];
            resultPath = argv[2];
            complete = stringToBool(argv[3]);

            A_MATRIX = Matrix(inputMatrix);
        } else if (argc == 7) {
            int rows = std::stoi(argv[1]);
            int cols = std::stoi(argv[2]);
            double min = std::stod(argv[3]);
            double max = std::stod(argv[4]);
            resultPath = argv[5];
            complete = stringToBool(argv[6]);

            A_MATRIX = Matrix(rows, cols, min, max);
        }
        cout << "Starting an MPI parallel matrix multiplication. \n  " << endl;

        numberOfRowsA = A_MATRIX.getRowsCount();  
        numberOfColsA = A_MATRIX.getColumnsCount();  

        A = flatten(A_MATRIX.getData());

        cout << "The matrices are: " << numberOfRowsA << "x" << numberOfColsA << endl;      

        /* Begin Timing: used for total multiplication time: communication + calculations  */    
        start_time = MPI_Wtime(); 	
        for (int i = 1; i < nProcesses; i++) { /* for each worker */
            // calculate granularity (-1 comes from excluding the master process)  
            granularity = (numberOfRowsA / (nProcesses - 1)); 
            rowStart = (i - 1) * granularity;

            if (((i + 1) == nProcesses) && ((numberOfRowsA % (nProcesses - 1)) != 0)) { //if rows of [A] cannot be equally divided among workers
                rowEnd = numberOfRowsA; //last worker gets all the remaining rows
            } else {
                rowEnd = rowStart + granularity; //rows of [A] are equally divisable among workers
            }

            //send the low bound, without blocking, to the intended worker 
            MPI_Isend(&rowStart, 1, MPI_INT, i, ROW_END_TAG, MPI_COMM_WORLD, &request);
            //next send the upper bound without blocking, to the intended worker
            MPI_Isend(&rowEnd, 1, MPI_INT, i ,ROW_START_TAG, MPI_COMM_WORLD, &request);

            //send matrices dimentions from the master
            MPI_Isend(&numberOfRowsA, 1, MPI_INT, i, A_TOTAL_ROWS_TAG, MPI_COMM_WORLD, &request);
            MPI_Isend(&numberOfColsA, 1, MPI_INT, i, A_COLS_TAG, MPI_COMM_WORLD, &request);

            //finally send the allocated row granularity of [A] without blocking, to the intended worker
            MPI_Isend(&A[rowStart * numberOfColsA], (rowEnd - rowStart) * numberOfColsA, MPI_DOUBLE, i, A_ROWS_TAG, MPI_COMM_WORLD, &request); 
        }
    }

    if (rank > 0) {   /* work done by workers (not rank = 0)*/

        //receive matrices dimentions from the master
        MPI_Recv(&numberOfRowsA, 1, MPI_INT, 0, A_TOTAL_ROWS_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&numberOfColsA, 1, MPI_INT, 0, A_COLS_TAG, MPI_COMM_WORLD, &status);

        //receive low bound from the master
        MPI_Recv(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &status);
        //next receive upper bound from the master
        MPI_Recv(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &status);

        vector<double> A_Local = vector<double>((rowEnd - rowStart)*numberOfColsA, 0.0);

        //finally receive row granularity of [A] to be processed from the master
        MPI_Recv(&A_Local[0], (rowEnd - rowStart) * numberOfColsA, MPI_DOUBLE, 0, A_ROWS_TAG, MPI_COMM_WORLD, &status);

        // start time for local time: the amount of time to do matrix calculation for this process
        localTimeSaver = MPI_Wtime(); 
        
        for (int i = 0; i < (rowEnd - rowStart); i++) {//the given set of rows of A (parallelized loop)
            partial_sort(A_Local.begin()+i*numberOfColsA, A_Local.begin() + i*numberOfColsA + numberOfColsA, A_Local.end());
        }
        // calculate local time: the amount of time to do matrix calculation for this process
        localTimeSaver = MPI_Wtime() - localTimeSaver;
        
        //send back the low bound first without blocking, to the master
        MPI_Isend(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &request);
        //send the upper bound next without blocking, to the master
        MPI_Isend(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &request);
        //finally send the processed granularity of data without blocking, to the master
        MPI_Isend(&A_Local[0], (rowEnd - rowStart) * numberOfColsA, MPI_DOUBLE, 0, C_ROWS_TAG, MPI_COMM_WORLD, &request);
        //send back the local calculation time without blocking, to the master
        MPI_Isend(&localTimeSaver, 1, MPI_DOUBLE, 0, LOCAL_TIME_TAG, MPI_COMM_WORLD, &request);
        

    }

    if (rank == 0) { /* master gathers processed work*/
        vector<double> C(numberOfColsA*numberOfRowsA);
        for (int i = 1; i < nProcesses; i++) {// untill all workers have handed back the processed data
            //receive low bound from a worker
            MPI_Recv(&rowStart, 1, MPI_INT, i, ROW_END_TAG, MPI_COMM_WORLD, &status);
            //receive upper bound from a worker
            MPI_Recv(&rowEnd, 1, MPI_INT, i, ROW_START_TAG, MPI_COMM_WORLD, &status);
            // //receive processed data from a worker
            MPI_Recv(&C[rowStart * numberOfColsA], (rowEnd - rowStart) * numberOfColsA, MPI_DOUBLE, i, C_ROWS_TAG, MPI_COMM_WORLD, &status);       
        }
        end_time = MPI_Wtime(); //end time of the total matrix matrix multiplication
        double totalMultiplicationTime = end_time - start_time;

        // find the longest local calulation (which we take as the total amount of calculation time, the rest comming from communication. 
        std::vector<double> LocalMultiplicationTimes = std::vector<double>(nProcesses);

        for (int i = 1; i < nProcesses; i++) {
        MPI_Recv(&LocalMultiplicationTimes[i], 1, MPI_DOUBLE, i, LOCAL_TIME_TAG, MPI_COMM_WORLD, &status); 
        }    
        double maxLocalMultiplicationTime = *std::max_element(LocalMultiplicationTimes.begin(), LocalMultiplicationTimes.end());

        // print out the results
        cout <<"Total multiplication time =  " << totalMultiplicationTime <<"\n"<< endl;       
        cout <<"Longest multiplication time =  " << maxLocalMultiplicationTime  <<"\n"<< endl;
        cout <<"Approximate communication time =  " << totalMultiplicationTime -maxLocalMultiplicationTime  <<"\n\n"<< endl;

        Matrix C_MATRIX = Matrix(numberOfRowsA, numberOfColsA, 0.0);
        for (int i = 0; i < numberOfRowsA; i++){
            for (int j = 0; j < numberOfColsA; j++){
                cout << C[i*numberOfColsA + j] << ";";
                C_MATRIX.setValue(i, j, C[i*numberOfColsA + j]);
            }
            cout << endl;
        }

        if (complete) {
            Matrix result = C_MATRIX.sortPartialySortedMatrix();
            result.writeToFile(resultPath);
        } else {
            C_MATRIX.writeToFile(resultPath);
        }
        
    }
    MPI_Finalize(); //finalize MPI operations
    return 0;
}