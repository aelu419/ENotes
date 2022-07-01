#include <iostream>
#include "person.h"
#include <boost/mpi.hpp>

int main(int argc, char *argv[])
{
    // switch to boost implementation
    boost::mpi::environment env;
    boost::mpi::communicator world;

    if (world.rank() == 0)
    {
        // note: person still needs to be serializable
        Person p{"bob", 2};
        for (size_t i = 1; i < world.size(); i++) {
            world.send(i, 0, p);
        }
    }
    else
    {
        Person p;
        world.recv(0, 0, p);
        std::cout << p.name << p.age << std::endl;
    }

    // note no finalize

    return 0;
}