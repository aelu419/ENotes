#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>

void shared() {
    int s = 10;
#pragma omp parallel for num_threads(4)
    for(int i = 0; i < 10; i++) {
        s = omp_get_thread_num();
    }

    std::cout<<"unreliable shared variable: "<<s<<std::endl;
}

void privateVar() {
    int s = 10;
#pragma omp parallel for num_threads(4) private(s)
    // note that this private variable s is not initialized (the 10 is not passed to the private copy)
    for(int i = 0; i < 10; i++) {
        s = omp_get_thread_num();
        std::cout<<"thread-local variable assigned to "<<s<<"\n";
    }

    std::cout<<"shared variable not modified: "<<s<<"\n";
}

void firstPrivate() {
    int s = 10;
#pragma omp parallel for num_threads(4) firstprivate(s)
    // note that here the 10 is passed to s
    for(int i = 0; i < 10; i++) {
        std::cout<<"thread-local variable of thread "<<omp_get_thread_num()<<" is set to "<<s<<"\n";
        ++s;
    }

    std::cout<<"shared variable not modified: "<<s<<"\n";
}

void lastPrivate() {
    int s = 10;

// note using firstprivate and lastprivate together
#pragma omp parallel for num_threads(4) firstprivate(s) lastprivate(s)
    // note that here the 10 is passed to s
    for(int i = 0; i < 10; i++) {
        std::cout<<"thread-local variable of thread "<<omp_get_thread_num()<<" is set to "<<s<<"\n";
        ++s;
    }

    std::cout<<"shared variable relies on the version on the thread that executed the last iteration: "<<s<<"\n";
}


int main(int argc, char *argv[])
{
    shared();
    privateVar();
    firstPrivate();
    lastPrivate();

    std::getchar();
    return 0;
}