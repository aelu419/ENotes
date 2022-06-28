#include <iostream>

#include <boost/optional.hpp>
#include "Person.hxx"
#include "Address.hxx"

int main(int argc, char *argv[])
{

    /**
     * @brief storing optinal values
     * vanilla C and (non-modern) CPP doesn't handle this well
     *   - no clear default values for each datatype
     *   - wastes space by storing a whole object as default, when in actuality it should be null
     *
     * use boost::optional<T> as alternative
     *   - an optional is not a pointer to
     *   - two different optionals never refer to the same value
     *   - construction
     *     - without any initialization expression, the optional is left empty without initializing to default
     *     - instance of T can be assigned to an optional
     *     - optional T can be copied
     *   - assignment
     *     - value type
     *     - reference (rebinds the wrapped reference to this optional)
     *     - another optional<T>
     *   - use dereference operator
     *     - undefined result when not holding value
     *   - to check for value, use
     *     - boost::optional<T>::is_initialized() -> bool
     *     - type conversion to bool (implicit, explicit, via operator!)
     *
     * note that the standard library also has an optional<T> type since CPP 17, prefer that instead
     *   if the project does not need to be pre-CPP 17
     */

    Person p{"Bob", "Bibo"};

    // this gives an error because optional<string> does not automatically convert to printable format
    // std::cout << p.middlename <<std::endl;

    // this gives an error because middlename is empty
    // sample output: [1]   8417 abort  ./Main
    // std::cout << *p.middlename <<std::endl;

    // this prints nothing
    if (p.middlename)
    {
        std::cout << *p.middlename << std::endl;
    }
    else
    {
        std::cout << "no middlename" << std::endl;
    }

    // assign T instance to optional
    p.middlename = "Babo";
    if (p.middlename)
    {
        std::cout << *p.middlename << std::endl;
    }

    // assign wrapped T instance to optional<T>
    p.middlename = boost::optional<std::string>{"Bubo"};
    if (p.middlename)
    {
        std::cout << *p.middlename << std::endl;
    }

    // using pointer dereference notation
    p.address = Address{"123 Any St", "Room 1"};
    if (p.address)
    {
        std::cout << p.address->line1 << std::endl;
    }

    return 0;
}