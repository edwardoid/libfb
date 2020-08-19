cmake_minimum_required(VERSION 3.5)
project(fb VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

if (TARGET_DEVICE)
  set(CMAKE_BUILD_TYPE Release)
  add_definitions(-DTARGET_DEVICE)
<<<<<<< HEAD
=======
  include_directories("$ENV{SYSROOT_DIR}/usr/include")
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
endif()

find_package(PkgConfig)

file(GLOB SOURCES "${CMAKE_CURRENT_LIST_DIR}/include/*.cpp")
file(GLOB INCLUDES "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

find_package(Threads)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

<<<<<<< HEAD
if (LIBFB_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/wifi_setup.h)
=======
add_library(${PROJECT_NAME} OBJECT ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/wifi_setup.h)
set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
install(TARGETS ${PROJECT_NAME}_static DESTINATION lib)
install(TARGETS ${PROJECT_NAME}_shared DESTINATION lib)

#export(TARGETS ${PROJECT_NAME}_shared ${PROJECT_NAME}_static FILE libfb-exports.cmake)
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd

if (BUILD_TEST)
   file(GLOB TEST_SOURCES "tests/*.cpp")
   add_executable(${PROJECT_NAME}_test ${SOURCES} ${TEST_SOURCES})
   include_directories(${PROJECT_NAME}_test "include")

<<<<<<< HEAD
   target_link_libraries (${PROJECT_NAME}_test ${PROJECT_NAME})
=======
   target_link_libraries (${PROJECT_NAME}_test ${PROJECT_NAME}_static)
>>>>>>> b5f68606ae5e04f9fe26aa0cd5bd8994a337e4dd
   target_link_libraries(${PROJECT_NAME}_test ${FREETYPE_LIBRARIES})
   target_link_libraries(${PROJECT_NAME}_test ${PNG_LIBRARIES})
   target_link_libraries(${PROJECT_NAME}_test ${CMAKE_THREAD_LIBS_INIT})
endif()
