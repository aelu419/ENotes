#include <iostream>
#include <string>
#include <boost/any.hpp>
#include <vector>
#include <typeinfo>

int main(int argc, char *argv[])
{

    /**
     * @brief any
     * used to provide functionality like "prototype object"
     *   - any T instance can be converted to boost::any<T>, provided T is copy-constructable
     *   - values stored as is and could be used in certain collections
     */

    boost::any a;      // T-less any
    boost::any b{2.0}; // any with inferred initial value
    std::vector<boost::any> v{1, 'a', "foo"};

    if (a.empty())
    {
        std::cout << "empty any's cannot be checked against" << std::endl;
    }

    // prints name of the type
    std::cout << "b is of a type with name " << b.type().name() << std::endl;

    // strict type matching works well
    double c = boost::any_cast<double>(b);

    try
    {
        int e = boost::any_cast<int>(b); // throws error when cannot cast
    }
    catch (boost::bad_any_cast &e)
    {
        std::cerr << "expected error occurrs...\n"
                  << e.what() << std::endl;
    }

    // using the pointer casting to 
    for (boost::any i : v) {
        // note that what is being casted is operator&(any) instead of the actual any
        int *val = boost::any_cast<int>(&i);
        if (val == nullptr) {
            std::cout<<"null pointer\n";
        } else {
            std::cout<<*val<<" can be casted to an int \n";
        }
    }

    return 0;
}