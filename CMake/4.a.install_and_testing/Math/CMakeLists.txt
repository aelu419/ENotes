add_library(Math arithmetics.cxx)
target_include_directories(Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# install the library and header file for this library
install(TARGETS Math DESTINATION lib)
install(FILES Math.h DESTINATION include)