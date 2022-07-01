#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /**
     * @brief for 5 processes, below outputs
     * 
        ready to send to 1
        received 1
        sent
        ready to send to 2
        received 1
        sent
        ready to send to 3
        sent
        ready to send to 4
        received 1
        received 1
        sent
     * 
     * - note that the next sent waits for the previous receive to complete
     */

    if (rank == 0)
    {
        int v = 1;
        for (size_t i = 1; i < size; i++) {
            std::cout << "ready to send to "<<i<<"\n";
            MPI_Ssend(&v, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            std::cout << "sent\n";
        }
    }
    else // if (rank != 4) <-- adding this condition would block the process on 0, because sending to 4 never completes
    {
        int n{};

        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout<<"received "<<n<<"\n";
    }

    MPI_Finalize();

    return 0;
}