#include <iostream>
#include <vector>
#include <mpi.h>
#include "Matrix.cpp"

using namespace std;

#define ROW_START_TAG 0    //tag for communicating the start row of the workload for a worker
#define ROW_END_TAG 1      //tag for communicating the end row of the workload for a worker
#define A_ROWS_TAG 2       //tag for communicating the address of the data to be worked on to worker
#define C_ROWS_TAG 3       //tag for communicating the address of the calculated data to master
#define LOCAL_TIME_TAG 4   //tag for communicating the address of the local matrix calculation time to master
#define B_ROWS_TAG 5       //tag for communicating the number of the rows in matrix B to worker 
#define B_COLS_TAG 6       //tag for communicating the number of the columns in matrix B to worker
#define A_ROWS_TOTAL_TAG 7 //tag for communicating the number of the rows in matrix A to worker
#define B_MATRIX_TAG 8     //tag for communicating the address of the data to be worked on to worker

double start_time, end_time;
double localTimeSaver;

std::vector<double> flatten(const std::vector<std::vector<double>>& matrix) {
    std::vector<double> flattened;
    for (const auto& row : matrix) {
        flattened.insert(flattened.end(), row.begin(), row.end());
    }
    return flattened;
}

int main(int argc, char *argv[]) {

    // Instantiate global variables used in the parallelization
    int rank;                  // mpi: process id number
    int nProcesses;            // mpi: number of total processess 
    MPI_Status status;         // mpi: store status of a MPI_Recv
    MPI_Request request;       // mpi: capture request of a MPI_Isend
    int rowStart, rowEnd;      // which rows of A that are calculated by the worker process
    int granularity; 	   // granularity of parallelization (# of rows per processor) 

    string file1 = argv[1];
    string file2 = argv[2];
    string file3 = argv[3];
    
    int numberOfRowsA;  
    int numberOfColsA;  
    int numberOfRowsB;  
    int numberOfColsB; 

    vector<double> A;
    vector<double> B;
    vector<double> C;

    MPI_Init(&argc, &argv);		                  /* initialize MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);	      /* store the rank */
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses); /* store the number of processes */

    if (rank == 0) {  /* Master initializes work*/
        cout << "Starting an MPI parallel matrix multiplication. \n  " << endl;

        Matrix A_MATRIX = Matrix(file1); 
        Matrix B_MATRIX = Matrix(file2); 

        numberOfRowsA = A_MATRIX.getRowsCount();  
        numberOfColsA = A_MATRIX.getColumnsCount();  
        numberOfRowsB = B_MATRIX.getRowsCount();  
        numberOfColsB = B_MATRIX.getColumnsCount(); 

        A = flatten(A_MATRIX.getData());
        B = flatten(B_MATRIX.getData());
        C = vector<double>(numberOfRowsA*numberOfColsB, 0.0);

        cout << "The matrices are: " << numberOfRowsA << "x" << numberOfColsA << " and " << numberOfRowsB << "x" << numberOfColsB << endl;      

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
            MPI_Isend(&numberOfRowsB, 1, MPI_INT, i, B_ROWS_TAG, MPI_COMM_WORLD, &request);
            MPI_Isend(&numberOfColsB, 1, MPI_INT, i ,B_COLS_TAG, MPI_COMM_WORLD, &request);
            MPI_Isend(&numberOfRowsA, 1, MPI_INT, i, A_ROWS_TOTAL_TAG, MPI_COMM_WORLD, &request);

            //finally send the allocated row granularity of [A] without blocking, to the intended worker
            MPI_Isend(&A[rowStart * numberOfColsA], (rowEnd - rowStart) * numberOfColsA, MPI_DOUBLE, i, A_ROWS_TAG, MPI_COMM_WORLD, &request); 
            // send matrix B to all workers
            MPI_Isend(&B[0], numberOfRowsB * numberOfColsB, MPI_DOUBLE, i, B_MATRIX_TAG, MPI_COMM_WORLD, &request);
        }
    }

    if (rank > 0) {   /* work done by workers (not rank = 0)*/

        //receive matrices dimentions from the master
        MPI_Recv(&numberOfRowsB, 1, MPI_INT, 0, B_ROWS_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&numberOfColsB, 1, MPI_INT, 0, B_COLS_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&numberOfRowsA, 1, MPI_INT, 0, A_ROWS_TOTAL_TAG, MPI_COMM_WORLD, &status);

        //initialize local versions of vectors 
        A = vector<double>(numberOfRowsA*numberOfRowsB, 0.0);
        C = vector<double>(numberOfRowsA*numberOfColsB, 0.0);
        B = vector<double>(numberOfRowsB*numberOfColsB, 0.0);

        //receive low bound from the master
        MPI_Recv(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &status);
        //next receive upper bound from the master
        MPI_Recv(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &status);
        //finally receive row granularity of [A] to be processed from the master
        MPI_Recv(&A[rowStart * numberOfRowsB], (rowEnd - rowStart) * numberOfRowsB, MPI_DOUBLE, 0, A_ROWS_TAG, MPI_COMM_WORLD, &status);
        //finally receive matrix [B] to be processed from the master
        MPI_Recv(&B[0], numberOfRowsB * numberOfColsB, MPI_DOUBLE, 0, B_MATRIX_TAG, MPI_COMM_WORLD, &status);

        // start time for local time: the amount of time to do matrix calculation for this process
        localTimeSaver = MPI_Wtime(); 
        
        for (int i = rowStart; i < rowEnd; i++) {//the given set of rows of A (parallelized loop)
            for (int j = 0; j < numberOfColsB; j++) {//iterate through columns of [B]
                for (int k = 0; k < numberOfRowsB; k++) {//iterate through rows of [B]
                    C[i*numberOfColsB+j] += (A[i*numberOfRowsB+k] * B[k*numberOfColsB+j]);
                }
            }
        }
        // calculate local time: the amount of time to do matrix calculation for this process
        localTimeSaver = MPI_Wtime() - localTimeSaver;
        
        //send back the low bound first without blocking, to the master
        MPI_Isend(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &request);
        //send the upper bound next without blocking, to the master
        MPI_Isend(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &request);
        //finally send the processed granularity of data without blocking, to the master
        MPI_Isend(&C[rowStart * numberOfColsB], (rowEnd - rowStart) * numberOfColsB, MPI_DOUBLE, 0, C_ROWS_TAG, MPI_COMM_WORLD, &request);
        //send back the local calculation time without blocking, to the master
        MPI_Isend(&localTimeSaver, 1, MPI_DOUBLE, 0, LOCAL_TIME_TAG, MPI_COMM_WORLD, &request);
        

    }

    if (rank == 0) { /* master gathers processed work*/
        for (int i = 1; i < nProcesses; i++) {// untill all workers have handed back the processed data
            //receive low bound from a worker
            MPI_Recv(&rowStart, 1, MPI_INT, i, ROW_END_TAG, MPI_COMM_WORLD, &status);
            //receive upper bound from a worker
            MPI_Recv(&rowEnd, 1, MPI_INT, i, ROW_START_TAG, MPI_COMM_WORLD, &status);
            // //receive processed data from a worker
            MPI_Recv(&C[rowStart * numberOfColsB], (rowEnd - rowStart) * numberOfColsB, MPI_DOUBLE, i, C_ROWS_TAG, MPI_COMM_WORLD, &status);     
            
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

        Matrix C_MATRIX = Matrix(numberOfRowsA, numberOfColsB, 0.0);
        for (int i = 0; i < numberOfRowsA; i++){
            for (int j = 0; j < numberOfColsB; j++){
                C_MATRIX.setValue(i, j, C[i*numberOfColsB + j]);
            }
        }
        C_MATRIX.writeToFile(file3);
        
    }
    MPI_Finalize(); //finalize MPI operations
    return 0;
}