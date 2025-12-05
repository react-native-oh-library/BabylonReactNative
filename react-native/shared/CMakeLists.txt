include(FetchContent)

FetchContent_Declare(babylonnative
    GIT_REPOSITORY https://gitee.com/openharmony-tpc-incubate/BabylonNative.git
    GIT_TAG c4db43de6cf113f91f29f19c70dffd30fd125319)

set(SHARED_INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}")

set(SHARED_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/DispatchFunction.h"
    "${CMAKE_CURRENT_LIST_DIR}/BabylonNative.h"
    "${CMAKE_CURRENT_LIST_DIR}/BabylonNative.cpp")

if (${BASEKIT_BUILD})
    set(BABYLON_NATIVE_PLUGIN_NATIVEXR OFF CACHE BOOL "")
    set(BABYLON_NATIVE_PLUGIN_NATIVECAMERA OFF CACHE BOOL "")
endif()
FetchContent_GetProperties(babylonnative)
if(NOT babylonnative_POPULATED)
    FetchContent_Populate(babylonnative)
endif()