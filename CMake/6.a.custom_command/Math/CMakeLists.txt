# add an executable that writes arg1 to a new file at arg2
add_executable(WriteNumberToFile write_number_to_file.cxx)
# call that executable
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
    COMMAND WriteNumberToFile 12345 ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
    DEPENDS WriteNumberToFile # mark dependency
)

# add dependency to the generated file for the Math library
add_library(Math arithmetics.cxx ${CMAKE_CURRENT_BINARY_DIR}/Generated.h)
# include directory for the generated file for the current target
target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
    #include <cmath>
    int main() {
        std::exp(1.0);
        return 0; 
    }
" EXP_AVAILABLE)

if (EXP_AVAILABLE)
    target_compile_definitions(Math PRIVATE "EXP_AVAILABLE")
endif()

install(TARGETS Math DESTINATION lib)
install(FILES Math.h DESTINATION include)