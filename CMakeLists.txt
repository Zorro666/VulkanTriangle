cmake_minimum_required(VERSION 2.8.12)

set(PROJECT VulkanTriangle)

set(COMMON_SRC
    3rdparty/volk/volk.h
    3rdparty/volk/volk.c
    3rdparty/VulkanMemoryAllocator/vk_mem_alloc.h
    apple/official/AppKit/NSMenuItem.hpp
    apple/official/AppKit/NSWindow.hpp
    apple/official/AppKit/AppKit.hpp
    apple/official/AppKit/NSRunningApplication.hpp
    apple/official/AppKit/NSView.hpp
    apple/official/AppKit/NSApplication.hpp
    apple/official/AppKit/NSMenu.hpp
    apple/official/AppKit/AppKitPrivate.hpp
    apple/official/Foundation/NSString.hpp
    apple/official/Foundation/NSDictionary.hpp
    apple/official/Foundation/NSLock.hpp
    apple/official/Foundation/NSProcessInfo.hpp
    apple/official/Foundation/NSError.hpp
    apple/official/Foundation/NSEvent.hpp
    apple/official/Foundation/NSObjCRuntime.hpp
    apple/official/Foundation/NSObject.hpp
    apple/official/Foundation/NSNotification.hpp
    apple/official/Foundation/NSPrivate.hpp
    apple/official/Foundation/NSURL.hpp
    apple/official/Foundation/NSAutoreleasePool.hpp
    apple/official/Foundation/NSRange.hpp
    apple/official/Foundation/NSTypes.hpp
    apple/official/Foundation/NSData.hpp
    apple/official/Foundation/NSNumber.hpp
    apple/official/Foundation/Foundation.hpp
    apple/official/Foundation/NSDate.hpp
    apple/official/Foundation/NSDefines.hpp
    apple/official/Foundation/NSBundle.hpp
    apple/official/Foundation/NSEnumerator.hpp
    apple/official/Foundation/NSArray.hpp
    apple/official/QuartzCore/CADefines.hpp
    apple/official/QuartzCore/CAMetalLayer.hpp
    apple/official/QuartzCore/CAPrivate.hpp
    apple/official/QuartzCore/QuartzCore.hpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/apple/official
   )

target_include_directories(VulkanTriangle PRIVATE ${INCLUDE_DIRS})
target_link_libraries(VulkanTriangle PRIVATE ${LIBS})
set_target_properties(VulkanTriangle PROPERTIES OUTPUT_NAME vulkan-triangle)

