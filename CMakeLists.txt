cmake_minimum_required(VERSION 2.8.12)

set(PROJECT VulkanTriangle)

set(COMMON_SRC
    3rdparty/volk/volk.h
    3rdparty/volk/volk.c
    3rdparty/VulkanMemoryAllocator/vk_mem_alloc.h
    official/metal-cpp-extensions/AppKit/NSMenuItem.hpp
    official/metal-cpp-extensions/AppKit/NSWindow.hpp
    official/metal-cpp-extensions/AppKit/AppKit.hpp
    official/metal-cpp-extensions/AppKit/NSRunningApplication.hpp
    official/metal-cpp-extensions/AppKit/NSView.hpp
    official/metal-cpp-extensions/AppKit/NSApplication.hpp
    official/metal-cpp-extensions/AppKit/NSMenu.hpp
    official/metal-cpp-extensions/AppKit/AppKitPrivate.hpp
    official/metal-cpp-extensions/Foundation/NSString.hpp
    official/metal-cpp-extensions/Foundation/NSDictionary.hpp
    official/metal-cpp-extensions/Foundation/NSLock.hpp
    official/metal-cpp-extensions/Foundation/NSProcessInfo.hpp
    official/metal-cpp-extensions/Foundation/NSError.hpp
    official/metal-cpp-extensions/Foundation/NSEvent.hpp
    official/metal-cpp-extensions/Foundation/NSObjCRuntime.hpp
    official/metal-cpp-extensions/Foundation/NSObject.hpp
    official/metal-cpp-extensions/Foundation/NSNotification.hpp
    official/metal-cpp-extensions/Foundation/NSPrivate.hpp
    official/metal-cpp-extensions/Foundation/NSURL.hpp
    official/metal-cpp-extensions/Foundation/NSAutoreleasePool.hpp
    official/metal-cpp-extensions/Foundation/NSRange.hpp
    official/metal-cpp-extensions/Foundation/NSTypes.hpp
    official/metal-cpp-extensions/Foundation/NSData.hpp
    official/metal-cpp-extensions/Foundation/NSNumber.hpp
    official/metal-cpp-extensions/Foundation/Foundation.hpp
    official/metal-cpp-extensions/Foundation/NSDate.hpp
    official/metal-cpp-extensions/Foundation/NSDefines.hpp
    official/metal-cpp-extensions/Foundation/NSBundle.hpp
    official/metal-cpp-extensions/Foundation/NSEnumerator.hpp
    official/metal-cpp-extensions/Foundation/NSArray.hpp
    official/metal-cpp-extensions/QuartzCore/CADefines.hpp
    official/metal-cpp-extensions/QuartzCore/CAMetalLayer.hpp
    official/metal-cpp-extensions/QuartzCore/CAPrivate.hpp
    official/metal-cpp-extensions/QuartzCore/QuartzCore.hpp
    vk/vk_helpers.cpp
    vk/vk_helpers.h
    vk/vk_test.cpp
    vk/vk_test.h
    vk/vk_simple_triangle.cpp
    vk/vk_multi_cmd_buffers.cpp
    test_common.cpp
    test_common.h
    apple/apple_platform.cpp
    apple/apple_platform.h
    apple/apple_window.cpp
    apple/apple_window.h
   )

set(SRC
     vulkan-triangle/main.cpp
   )

project(VulkanTriangle)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
list(APPEND warning_flags -Werror)
string(REPLACE ";" " " warning_flags "${warning_flags}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warning_flags}")

add_executable(VulkanTriangle ${SRC} ${COMMON_SRC})

install(TARGETS VulkanTriangle DESTINATION .)

find_library(COCOA_LIBRARY Cocoa)
find_library(QUARTZCORE_LIBRARY QuartzCore)
target_compile_definitions(VulkanTriangle PRIVATE -DVK_USE_PLATFORM_MACOS_MVK=1)
set(LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${QUARTZCORE_LIBRARY})

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    /Users/jake/Workspace/personal/MoltenVK/MoltenVk/include
    ${CMAKE_CURRENT_SOURCE_DIR}/official/metal-cpp-extensions
   )

target_include_directories(VulkanTriangle PRIVATE ${INCLUDE_DIRS})
target_link_libraries(VulkanTriangle PRIVATE ${LIBS})
set_target_properties(VulkanTriangle PROPERTIES OUTPUT_NAME vulkan-triangle)

