#include <iostream>
#include <mpi.h>
#include "person.h"
#include <sstream>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {

        Person p{"bob", 2};

        // serialize
        std::ostringstream oss;
        boost::archive::text_oarchive ar(oss);
        ar << p;

        std::string s = oss.str();
        int len = s.length() + 1; // account for \0

        std::cout << "sending person " << p.name << " of age " << p.age << " as serialized " << s.c_str() << " (buffer length " << len << ")" << std::endl;

        // send serialized custom object
        for (size_t i = 1; i < size; i++)
        {
            MPI_Send(s.c_str(), len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        // note: this receive does not specify the count of received data
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);
        std::cout << "expecting an incoming string of size " << count << "\n";

        char *buffer = new char[count];

        // note the use of status instead of _IGNORE
        MPI_Recv(buffer, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        std::cout << buffer << std::endl;

        // deserialize
        std::istringstream iss(buffer);
        boost::archive::text_iarchive ar(iss);

        Person p;
        ar >> p;

        std::cout << "received person of name " << p.name << " and age " << p.age << "\n";

        delete[] buffer;
    }

    MPI_Finalize();

    return 0;
}