cmake_minimum_required(VERSION 3.12)
project(main)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenCV REQUIRED)

add_executable(
    main
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/common.cpp    
)

target_link_libraries(
    main
    ${OpenCV_LIBS}
)

