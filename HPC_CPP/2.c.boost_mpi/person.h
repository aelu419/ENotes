#ifndef person_h
#define person_h

#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Person {
public:
    std::string name = "";
    int age = -1;

    Person() = default;
    Person(std::string name, int age): name{name}, age{age}{}

    // conforming to boost serialization
    friend class boost::serialization::access;
    template <typename Ar> void serialize(Ar& ar, const unsigned int version) {
        ar & name;
        ar & age;
    }
};

#endif