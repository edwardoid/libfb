cmake_minimum_required(VERSION 3.5)
project(fb VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

if (TARGET_DEVICE)
  set(CMAKE_BUILD_TYPE Release)
  add_definitions(-DTARGET_DEVICE)
endif()

find_package(PkgConfig)

file(GLOB SOURCES "${CMAKE_CURRENT_LIST_DIR}/include/*.cpp")
file(GLOB INCLUDES "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

find_package(Threads)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

if (LIBFB_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/wifi_setup.h)

if (BUILD_TEST)
   file(GLOB TEST_SOURCES "tests/*.cpp")
   add_executable(${PROJECT_NAME}_test ${SOURCES} ${TEST_SOURCES})
   include_directories(${PROJECT_NAME}_test "include")

   target_link_libraries (${PROJECT_NAME}_test ${PROJECT_NAME})
   target_link_libraries(${PROJECT_NAME}_test ${FREETYPE_LIBRARIES})
   target_link_libraries(${PROJECT_NAME}_test ${PNG_LIBRARIES})
   target_link_libraries(${PROJECT_NAME}_test ${CMAKE_THREAD_LIBS_INIT})
endif()
