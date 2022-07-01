#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int size, rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /**
     * @brief broadcasting
     */

    int a{3};
    // note the `root` param after data type that specifies the broadcaster
    // everyone else receives
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << rank << " received " << a << "\n";

    /**
     * @brief distributed data handling via patterns like reduce, scatter, etc.
     */
    const int per = 100;
    size_t count = size * per;

    int *data = new int[count];
    if (rank == 0)
    {
        for (size_t i = 0; i < count; ++i)
        {
            data[i] = 1;
        }
    }

    int *localData = new int[per];

    // send buffer, send count (per process), send type
    // recv buffer, recv count, recv type
    // root, communicator
    MPI_Scatter(data, per, MPI_INT, localData, per, MPI_INT, 0, MPI_COMM_WORLD);

    int localSum = 0;
    for (size_t i = 0; i < per; i++)
    {
        localSum += localData[i];
    }
    delete[] localData;

    std::cout<<"local sum "<< localSum << "\n";

    int globalSum = 0;

    // local variable, global variable, send count (units per process, in this case only 1 local sum per process)
    // type, MPI operation (predefined SUM), root, communicator
    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0)
    {
        std::cout << "Total sum = " << globalSum << "\n";
    }

    delete[] data;

    /**
     * @brief barrier
     */

    MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "process finished\n";

    MPI_Finalize();

    return 0;
}