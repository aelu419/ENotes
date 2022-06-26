add_library(Math Math.cxx)

# the following line is modified to solve the error thrown by the EXPORT (see below)
# target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Math INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>)
# this makes the include directory conditional
#   for building within the project, it references the non-portable source directory
#   for installation, it references the include directory

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

# note the addition of EXPORT
# this creates a MathTargets.cmake file upon building
install(TARGETS ${installable_libs} DESTINATION lib EXPORT MathTargets)

# using the above statement alone, the library itself is still dependent on the current (non-portable) path
# which makes CMake give the following error:
# CMake Error in Math/CMakeLists.txt:
#   Target "Math" INTERFACE_INCLUDE_DIRECTORIES property contains path:
#     "/Users/alantao/Desktop/notes/CMake/11.a.export_configuration/Math"
#   which is prefixed in the source directory.

install(FILES Math.h DESTINATION include)