#include <iostream>

// certain libraries in Boost are *headers only*
//   all macros, functions, and headers are visible to the compiler via some header file
// other libraries require building Boost first and then including the library

#include <boost/array.hpp>
#include <boost/regex.hpp>

int main(int argc, char *argv[]) {

    boost::array<int, 10> a;
    boost::regex r("hello world");

    return 0;
}