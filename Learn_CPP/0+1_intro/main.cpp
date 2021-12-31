#include <iostream> // a preprocessor directive indicating we are using the contents of iostream library

/**
 * Chapters 0 and 1 of LearnCPP
 * @author Alan Tao
 * @version 1 12/30/21
 */

int main()
{ // main function as entry point to the program

    // function body
    // @chapter: 0
    std::cout << "Hello World" << std::endl; // print (send text "to" std::cout)

    // definition, initialization and standard IO
    // @chapter: 1

    /* ---------------------------------------

    This segment is written to default language standard of vscode
    with some language features in LearnCPP not supported

    // value initialization
    int x; 
    // int y(); // not allowed
    // int z{}; // not allwed
    std::cout << x; // not initialized to 0!

    // not allowed
    // int x { 5 };
    std::cout << "Hello world" << x;
    --------------------------------------- */

    // below are written to c++20
    int x = 8;
    // int y(); // not allowed
    int y(1); // allowed, zero init
    int z{};  // allowed, zero init
    int w{5};

    // output
    std::cout << x << " " << y << " " << z << " " << w << std::endl;
    std::cout << "slash n does the same thing as endl, but without flushing the output\n";

    // input
    std::cin >> x >> y;
    std::cout << x << " " << y << "\n";

    return 0; // exit point of the program
    //
}