cmake_minimum_required(VERSION 3.12)
PROJECT(main)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

FIND_PACKAGE(OpenCV REQUIRED)

ADD_EXECUTABLE(
    main
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/common.cpp    
)

TARGET_LINK_LIBRARIES(
    main
    ${OpenCV_LIBS}
)

