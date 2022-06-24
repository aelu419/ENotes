add_library(Math arithmetics.cxx)
target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# check if certain operations are available on the platform
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
    #include <cmath>
    int main() {
        std::exp(1.0);
        return 0; 
    }
" EXP_AVAILABLE)

if (EXP_AVAILABLE)
    target_compile_definitions(Math PRIVATE "EXP_AVAILABLE") # see use of this word in arithmetics.cxx
endif()

install(TARGETS Math DESTINATION lib)
install(FILES Math.h DESTINATION include)