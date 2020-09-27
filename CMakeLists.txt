cmake_minimum_required(VERSION 3.10)
project(slam)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(MODULE_DIR ${CMAKE_SOURCE_DIR}/module)

# opencv 
set(OpenCV_DIR ${MODULE_DIR}/OpenCV)
find_package(OpenCV REQUIRED)

# dbow3 
set(DBoW3_INCLUDE_DIRS ${MODULE_DIR}/DBow3/include)
set(DBoW3_LIB_DIR ${MODULE_DIR}/DBow3/lib)
include_directories(${DBoW3_INCLUDE_DIRS})
link_directories(${DBoW3_LIB_DIR})
set(DBoW3_LIBS DBoW3001) 

# glob
set(Glob_DIR ${MODULE_DIR}/Glob/lib/cmake/Glob-1.0)
set(Glob_LIB_DIR ${MODULE_DIR}/Glob/lib/Glob-1.0)
include_directories(${MODULE_DIR}/Glob/include/Glob-1.0)
link_directories(${Glob_LIB_DIR})
set(Glob_LIBS glob)

# copy dependencies
file(GLOB OpenCV_DLLS ${OpenCV_DIR}/x86/vc16/bin/*.dll)
if(CMAKE_BUILD_TYPE STREQUAL Release)
file(COPY ${OpenCV_DLLS} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
else()
file(COPY ${OpenCV_DLLS} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
endif()


add_executable(feature_training example/feature_training.cpp)
target_link_libraries(feature_training ${OpenCV_LIBS} ${DBoW3_LIBS})

add_executable(loop_closure example/loop_closure.cpp)
target_link_libraries(loop_closure ${OpenCV_LIBS} ${DBoW3_LIBS})

add_executable(gen_vocab example/gen_vocab_large.cpp)
target_link_libraries(gen_vocab ${OpenCV_LIBS} ${DBoW3_LIBS})

add_executable(show_match_result example/show_match_result.cpp)
target_link_libraries(show_match_result ${OpenCV_LIBS} ${DBoW3_LIBS})

# aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILES)
# add_executable(${PROJECT_NAME} ${SRC_FILES})
# target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${DBoW3_LIBS} ${Glob_LIBS})
