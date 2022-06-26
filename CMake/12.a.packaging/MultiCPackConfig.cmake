# this file is created by cmake by default
include("release/CPackConfig.cmake")

set(CPack_INSTALL_CMAKE_PROJECTS
    "debug;App;ALL;/"
    "release;App;All;/"
)

# run `cpack --config MultiCPackConfig.cmake` under the source directory