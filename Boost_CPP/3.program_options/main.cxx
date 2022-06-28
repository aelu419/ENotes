#include <iostream>
#include <string>
#include <boost/program_options.hpp>

// shorthand
namespace po = boost::program_options;

int main(int argc, char *argv[])
{

    /**
     * @brief program options
     * used to interpret flags and other inputs of the command
     * support for required/optional inputs, abbrieviated names, self-documenting, external configuration file
     *
     * fundamentals
     *   - options_description: the root container for options
     *      - allowed optinos to be declared in it
     *      - can be printed
     *      - add_options()
     *      - custom prefix
     *      - fluent interface
     *   - variables_map: stores parsed variables
     *      - vm.count("<vname>") counts the # of "<vname>" arguments, which may be > 1 in some cases
     *      - vm["<vname>"] retrieves the value of the argument
     *   - store(): gets a parser to parse args and store in vm
     *   - notify(): must be called after parsing, often used for adv. scenarios
     */

    po::options_description desc("program options");

    // note use of fluent
    // add_options() returns the &options_description but the method invocation is saved
    // because operator() is overriden
    desc.add_options()("help", "print info")                                                    // by default, prefix "--"
        ("filename", po::value<std::string>()->default_value("default.txt"), "file to process") // default value is also fluent
        ;

    po::variables_map vm;
    auto parsed = po::parse_command_line(argc, argv, desc);

    po::store(parsed, vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        // printing the object_description returns a self-documentation
        std::cout << desc << std::endl;

        // this prints:
        // program options:
        //   --help                        print info
        //   --filename arg (=default.txt) file to process
    }
    else
    {
        // note the check for filename is not needed because default values are provided
        // implicitly, variables_map uses boost::any
        // variables_map::as<T> converts the boost::any back to T form
        auto f = vm["filename"].as<std::string>();
        std::cout << "processing file " << f << std::endl;
    }

    return 0;
}