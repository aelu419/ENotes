#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>

void atomic()
{
    int sum = 0;
#pragma omp parallel for num_threads(128)
    for (int i = 0; i < 100; i++)
    {
        ++sum;
    }
    std::cout << "Note the first sum may not exactly be 100: " << sum << std::endl;
    sum = 0;
#pragma omp parallel for num_threads(128)
    for (int i = 0; i < 100; i++)
    {

// note the addition of the atomic keyword above the instruction itself
#pragma omp atomic
        ++sum;
    }
    std::cout << "Now the sum should be 100: " << sum << std::endl;

}

void order() {
    std::cout<<"out of order printing:"<<std::endl;
#pragma omp parallel for num_threads(16)
    for (int i = 0; i < 16; i++) {
        std::cout<<omp_get_thread_num()<<"\n";
    }

    std::cout<<"in order printing:"<<std::endl;

// note the addition of the ordered keyword in the loop
#pragma omp parallel for ordered num_threads(16)
    for (int i = 0; i < 16 ; i++) {
// note the addition of the ordered keyword above the instruciton itself
#pragma omp ordered
        std::cout<<omp_get_thread_num()<<"\n";
    }
}

void nowait() {
#pragma omp parallel num_threads(4);
{
#pragma omp single nowait
    {
        int a;
        std::cout<<"give me a number:\n";
        std::cin>>a;
        std::cout<<"the number is "<<a<<"\n";
    }

    // note that this part is AFTER the input, but it could complete on other threads before the input completes
    // this is because those other threads do not WAIT for the single thread handling the input
    std::cout<<"common workload on thread "<<omp_get_thread_num()<<"\n";
#pragma omp barrier

    // again, master only runs after barrier, meaning that every thread completed their common workload
#pragma omp master
    {
        std::cout<<"barrier reached, meaning that every thread should have completed their common workload\n";
    }

}
}

int main(int argc, char *argv[])
{
    atomic();
    order();
    nowait();

    std::getchar();
    return 0;
}