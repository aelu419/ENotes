#ifndef Address_h
#define Address_h

#include <boost/optional.hpp>
#include <string>

class Address
{
public:
    std::string line1;
    boost::optional<std::string> line2;
    Address(std::string line1, std::string line2) : line1{line1}, line2{line2} {}
};

#endif