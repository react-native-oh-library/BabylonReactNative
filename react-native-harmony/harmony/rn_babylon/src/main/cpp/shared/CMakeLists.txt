include(FetchContent)

FetchContent_Declare(babylonnative
    GIT_REPOSITORY https://gitee.com/openharmony-tpc-incubate/BabylonNative.git
    GIT_TAG 3f9f9160f3d61f58a94b9a73d6d08bf1b156d427)

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