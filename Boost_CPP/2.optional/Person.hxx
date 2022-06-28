#ifndef Person_h
#define Person_h

#include <string>
#include <boost/optional.hpp>
#include "Address.hxx"

class Person
{
public:
    std::string firstname, lastname;
    boost::optional<std::string> middlename;
    boost::optional<Address> address;

    Person(std::string f, std::string l) : firstname{f}, lastname{l} {}
};

#endif