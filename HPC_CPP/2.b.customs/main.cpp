#include <iostream> 
#include <mpi.h>

int main(int argc, char* argv[]) {

    /**
     * concepts
     *  - communicator: group of processes, by default MPI_COMM_WORLD
     * `- rank: position of the processes within the communicator, 0 indexed, always contiguous
     * api prefixed by MPI_
     * MPI uses its own typedefs
     */

    // note MPI is installed via HomeBrew

    MPI_Init(&argc, &argv);

    // actual MPI program
    int size, rank;

    // find the size and rank of this process within the communicator, then outputting then to appropriate variables
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "Enter a number: ";
        int n;
        std::cin >> n;

        // send number to every OTHER process
        // note that @i starts at 1
        for (size_t i = 1; i < size; i++) {
            /**
             * @brief sends the 
             * &n number to send
             * 1 count
             * MPI_INT type
             * i destination rank, up to size
             * 0 tag
             * MPI_COMM_WORLD use default communicator
             */
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // receive a number
        int n;
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // note the cout is *proxied* by MPI to show on the local process, even if the receiver is another process
        std::cout << n << "*" << rank << " = " << n * rank << std::endl;
    }

    MPI_Finalize();

    return 0;

}