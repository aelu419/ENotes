
cmake_minimum_required(VERSION 3.10)
project(App VERSION 0.1.0)

# passed to Config.h
# note this option is visible under ccmake or cmake-gui
option(USE_MATH "Use custom math implementation" ON)
configure_file(Config.h.in Config.h)

# optionally use the custom math library
if(USE_MATH)
    add_subdirectory(Math)
    list(APPEND EXTRA_LIBS Math)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Math")
endif()

# register main target
add_executable(App app.cxx)

# optionally add library and includes to main target
target_link_libraries(App PUBLIC ${EXTRA_LIBS})
target_include_directories(App PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})
