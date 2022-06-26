add_library(Math Math.cxx)

# new properties for debug
set_property(TARGET Math PROPERTY VERSION "1.0.0")
set_property(TARGET Math PROPERTY SOVERSION "1")

target_include_directories(Math INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>)

option(USE_MATH "Use custom math implementation" ON)

if(USE_MATH)
    target_compile_definitions(Math INTERFACE "USE_MATH")

    add_executable(WriteNumberToFile write_number_to_file.cxx)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
        COMMAND WriteNumberToFile 12345 ${CMAKE_CURRENT_BINARY_DIR}/Generated.h
        DEPENDS WriteNumberToFile
    )

    add_library(ArithmeticLibrary STATIC arithmetics.cxx ${CMAKE_CURRENT_BINARY_DIR}/Generated.h)
    target_include_directories(ArithmeticLibrary PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    set_target_properties(ArithmeticLibrary PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
    
    target_link_libraries(Math PRIVATE ArithmeticLibrary)
endif()

target_compile_definitions(Math PRIVATE "EXPORTING_MATH")

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
    #include <cmath>
    int main() {
        std::exp(1.0);
        return 0; 
    }
" EXP_AVAILABLE)

if (EXP_AVAILABLE)
    target_compile_definitions(ArithmeticLibrary PRIVATE "EXP_AVAILABLE")
endif()

set(installable_libs Math)
if(TARGET ArithmeticLibrary)
    list(APPEND installable_libs ArithmeticLibrary)
endif()

install(TARGETS ${installable_libs} DESTINATION lib EXPORT MathTargets)

install(FILES Math.h DESTINATION include)