# register the Math library with the *new* Math source
add_library(Math Math.cxx)

# declare that Math.h is located in the current source directory
target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# replaces the USE_MATH at the top level
option(USE_MATH "Use custom math implementation" ON)

# conditionally compile the Math library
if(USE_MATH)
    target_compile_definitions(Math PRIVATE "USE_MATH")

    # load header file generator
    add_executable(WriteNumberToFile write_number_to_file.cxx)

    # similarly, generate thet constant
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
        COMMAND WriteNumberToFile 12345 ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
        DEPENDS WriteNumberToFile # mark dependency
    )

    # move actual functionality to arithmetic library
    add_library(ArithmeticLibrary STATIC arithmetics.cxx ${CMAKE_CURRENT_BINARY_DIR}/Generated.h)
    target_include_directories(ArithmeticLibrary PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    set_target_properties(ArithmeticLibrary PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
    
    # link the functionality to the Math library
    target_link_libraries(Math PRIVATE ArithmeticLibrary)
endif()

# when building on windows, this symbol is used
target_compile_definitions(Math PRIVATE "EXPORTING_MATH")

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
    #include <cmath>
    int main() {
        std::exp(1.0);
        return 0; 
    }
" EXP_AVAILABLE)

# the constant use is moved to arithmetics
if (EXP_AVAILABLE)
    target_compile_definitions(ArithmeticLibrary PRIVATE "EXP_AVAILABLE")
endif()

# install rules for the Math library
set(installable_libs Math)
# optionally  also install the arithmetic functions
if(TARGET ArithmeticLibrary)
    list(APPEND installable_libs ArithmeticLibrary)
endif()
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES Math.h DESTINATION include)