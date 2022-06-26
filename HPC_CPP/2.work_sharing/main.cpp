#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>

void ompfor()
{
    std::vector<int> a(1024);
#pragma omp parallel for
    for (size_t i = 0; i < a.size(); ++i)
    {
        ++a[i];
    }
}

void ompsection()
{
    std::vector<int> a(1024);
    std::vector<char> b(1024);

// the tutorial did not have to specify num_threads but for some reason specification is needed on the working platform
// the correct result should have numbers mixed with letters to show that both sections are executing in parallel
#pragma omp parallel sections num_threads(2)
    {
#pragma omp section
        {
            for (size_t i = 0; i < a.size(); ++i)
            {
                ++a[i];
                std::cout << a[i];
            }
        }
#pragma omp section
        {
            for (size_t i = 0; i < b.size(); ++i)
            {
                b[i] = i % 26 + 'a';
                std::cout << b[i];
            }
        }
    }
    std::cout << std::endl;
}

void ompsingle()
{
#pragma omp parallel
    {
#pragma omp single
        {
            std::cout << "This should only be processed once, although there are " << omp_get_num_threads() << " threads.\n";
        }

        {
            std::cout << "This executes on all threads\n";
        }

#pragma omp barrier
#pragma omp master
        {
            std::cout << "Master thread says everything is done\n";
        }
    }
}

int main(int argc, char *argv[])
{
    ompfor();
    ompsection();
    ompsingle();

    std::getchar();
    return 0;
}