cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

# Configure Babylon Native to use JSI
set(NAPI_JAVASCRIPT_ENGINE "JSI" CACHE STRING "The JavaScript engine to power N-API" FORCE)

#  处理 error: unknown type name 'napi_type_tag'......
add_definitions(-DNAPI_VERSION=10)

set(BABYLON_REACT_NATIVE_SHARED_DIR "${NODE_MODULES}/@react-native-oh-tpl/babylonjs-react-native/shared")

include(${BABYLON_REACT_NATIVE_SHARED_DIR}/CMakeLists.txt)

add_subdirectory("${BABYLON_REACT_NATIVE_SHARED_DIR}" ./babaylonjsshared)
add_subdirectory(${babylonnative_SOURCE_DIR} ./babylonnaitve)

file(GLOB_RECURSE rnoh_babylon_SRC CONFIGURE_DEPENDS *.cpp)
add_library(rnoh_babylon SHARED ${rnoh_babylon_SRC}
                                ${SHARED_SOURCES}
)

target_include_directories(rnoh_babylon PUBLIC
    ${SHARED_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR})

find_library(
    # 设置路径变量的名称。
    EGL-lib
    # 指定要让CMake查找的NDK库的名称。
    EGL
)

find_library(
    # 设置路径变量的名称。
    GLES-lib
    # 指定要让CMake查找的NDK库的名称。
    GLESv3
)

target_link_libraries(rnoh_babylon PUBLIC rnoh
                                     libace_ndk.z.so
                                     libhilog_ndk.z.so
                                     libnative_drawing.so
                                     libnative_window.so
                                     libffrt.z.so
                                     ${EGL-lib} ${GLES-lib}
                                     NativeInput
                                     NativeOptimizations
                                     GraphicsDevice
                                     NativeTracing
                                     NativeCamera
                                     NativeXr
                                     NativeCapture
                                     NativeEngine
                                     Window
                                     Canvas
                                     XMLHttpRequest
                                     JsRuntime
                                     pixelmap
                                     image_packer
)
