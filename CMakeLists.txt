cmake_minimum_required(VERSION 3.5)
project(fb VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig)
find_package(Freetype REQUIRED)

find_package(PNG REQUIRED)

file(GLOB SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

if (LIBFB_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER HEADERS)

INSTALL(TARGETS ${PROJECT_NAME} 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)


include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${PNG_INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)


if (BUILD_TEST)
    find_package(Threads REQUIRED)
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
    include_directories(${PROJECT_NAME}_test "include")

    target_link_libraries (${PROJECT_NAME}_test qrencode)
    target_link_libraries (${PROJECT_NAME}_test ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}_test ${FREETYPE_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}_test ${PNG_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}_test ${CMAKE_THREAD_LIBS_INIT})
endif()
