cmake_minimum_required(VERSION 3.10)
SET(CMAKE_C_COMPILER C:/MinGW/bin/gcc.exe)
project("SocketServer")

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(BUILD_DEPS "YES" CACHE BOOL "If YES, will install dependencies into sandbox.  Automatically reset to NO after dependencies are installed.")
set(PROJECT_OUTPUT_DIR  ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

#---------Project--------
include_directories(${CMAKE_SOURCE_DIR}/include)
#aux_source_directory(./src SRC)

add_executable(${PROJECT_NAME} src/server.cpp)
#add_library( ${PROJECT_NAME} SHARED ${SRC} ${SRC_COMPONENT} ${SRC_ERRORCODE})
#add_library( ${PROJECT_NAME} STATIC ${SRC} ${SRC_COMPONENT} ${SRC_ERRORCODE})

IF (WIN32)
target_link_libraries(${PROJECT_NAME} WS2_32)
  # opencv_world349.lib libbreakpad.lib libbreakpad_client.lib ${FFMPEG_ALL_LIBS} Qt5Guid.lib Qt5Cored.lib Qt5Multimediad.lib Qt5Networkd.lib
  # libbreakpad.lib libbreakpad_client.lib 
  # boost_system-vc141-mt-x64-1_66.lib libboost_filesystem-vc141-mt-x64-1_66.lib libboost_regex-vc141-mt-x64-1_66.lib
ELSE()
target_link_libraries(${PROJECT_NAME} -lstdc++fs )
  # ${OpenCV_LIBS}
  # -lgomp -lboost_system -lboost_filesystem -lboost_regex ${Boost_LIBRARIES}
ENDIF()

#=======================================
project("SocketClient")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
add_executable(${PROJECT_NAME} src/client.cpp)
if (WIN32)
  target_link_libraries(${PROJECT_NAME} WS2_32)
endif()