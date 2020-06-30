cmake_minimum_required(VERSION 3.5)
project(fb VERSION 1.0 DESCRIPTION "A library to draw on Linux Framebuffer")
set(CMAKE_CXX_STANDARD 11)

if (TARGET_DEVICE)
  set(CMAKE_BUILD_TYPE Release)
  add_definitions(-DTARGET_DEVICE)
  include_directories("$ENV{SYSROOT_DIR}/usr/include")
endif()

find_package(PkgConfig)

file(GLOB SOURCES "include/*.cpp")
file(GLOB INCLUDES "include/*.h")

find_package(Threads)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/wifi_setup.h)
set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
#install(FILES "include/wifi_setup.h" DESTINATION include)
#install(FILES "3dp/s2s/s2s.h" DESTINATION include)
#install(FILES "3dp/s2s/s2s_property.h" DESTINATION include)
install(TARGETS ${PROJECT_NAME}_static DESTINATION lib)
install(TARGETS ${PROJECT_NAME}_shared DESTINATION lib)

if (BUILD_TEST)
   file(GLOB TEST_SOURCES "tests/*.cpp")
   add_executable(${PROJECT_NAME}_test ${SOURCES} ${TEST_SOURCES})
   include_directories(${PROJECT_NAME}_test "include")

   target_link_libraries (${PROJECT_NAME}_test ${PROJECT_NAME}_static)
   target_link_libraries(${PROJECT_NAME}_test ${FREETYPE_LIBRARIES})
   target_link_libraries(${PROJECT_NAME}_test ${PNG_LIBRARIES})
   target_link_libraries(${PROJECT_NAME}_test ${CMAKE_THREAD_LIBS_INIT})
endif()
