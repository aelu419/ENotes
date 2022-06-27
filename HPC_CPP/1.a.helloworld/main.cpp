#include <iostream>
#include <chrono>

#include <omp.h>

void hello_openmp()
{
    // a simple parallel that runs the same thing on each available core, without constraints
#pragma omp parallel
    {
        std::cout << "Hello, OpenMP \n";
    }

    // set to an arbitrary thread count
    omp_set_num_threads(4);
#pragma omp parallel
    {
        std::cout << "This is the " << omp_get_thread_num() + 1 << " / " << omp_get_num_threads() << "\n";
        // note that this prints out of order
    }

    // resume regular threadcount, this ensures the threads enter in order
    omp_set_num_threads(omp_get_num_procs());
#pragma omp parallel
    {
#pragma omp critical
        std::cout << "This is the " << omp_get_thread_num() + 1 << " / " << omp_get_num_threads() << " in order \n";
    }
}

int main(int argc, char *argv[])
{
    hello_openmp();
    std::getchar();
    return 0;
}