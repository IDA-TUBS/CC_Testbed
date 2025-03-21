cmake_minimum_required(VERSION 3.0.0)
project(cc_testbed VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

### Build directory structure
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

find_package(w2rp REQUIRED)
find_package(cc_linkmonitor REQUIRED)
find_package(Boost 1.73.0 EXACT COMPONENTS thread log log_setup REQUIRED)

# -------------------------------------------------------------------------- #
#                                Access Point                                #
# -------------------------------------------------------------------------- #

# -------------------------------- Downlink -------------------------------- #
add_executable(accessPoint_down src/accessPoint/downlink/accessPoint_down.cpp)
target_compile_options(accessPoint_down PUBLIC -g)
target_link_libraries(accessPoint_down ${OpenCV_LIBS} w2rp cc_linkmonitor pthread boost_log boost_log_setup)
target_compile_definitions(accessPoint_down PUBLIC BOOST_LOG_DYN_LINK LOG_ON FILE_ON CONSOLE_ON)

add_executable(accessPointGUI_down src/accessPoint/downlink/accessPointGUI_down.cpp)
target_compile_options(accessPointGUI_down PUBLIC -g)
target_include_directories(accessPointGUI_down PRIVATE ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(accessPointGUI_down ${OpenCV_LIBS} w2rp cc_linkmonitor pthread Qt5::Widgets Qt5::Core boost_log boost_log_setup)
target_compile_definitions(accessPointGUI_down PUBLIC BOOST_LOG_DYN_LINK LOG_ON FILE_ON CONSOLE_ON)

# -------------------------------- Uplink -------------------------------- #
add_executable(accessPoint_up src/accessPoint/uplink/accessPoint_up.cpp)
target_compile_options(accessPoint_up PUBLIC -g)
target_link_libraries(accessPoint_up ${OpenCV_LIBS} w2rp cc_linkmonitor pthread boost_log boost_log_setup)
target_compile_definitions(accessPoint_up PUBLIC BOOST_LOG_DYN_LINK LOG_ON FILE_ON CONSOLE_ON)

# -------------------------------------------------------------------------- #
#                                Mobile Node                                 #
# -------------------------------------------------------------------------- #

# -------------------------------- Downlink -------------------------------- #

add_executable(mobileNode_down src/mobileNode/downlink/mobileNode_down.cpp)
target_compile_options(mobileNode_down PUBLIC -g)
target_link_libraries(mobileNode_down ${OpenCV_LIBS} w2rp cc_linkmonitor pthread boost_log boost_log_setup)
target_compile_definitions(mobileNode_down PUBLIC BOOST_LOG_DYN_LINK LOG_ON FILE_ON CONSOLE_ON)

add_executable(mobileNodeGUI_down src/mobileNode/downlink/mobileNodeGUI_down.cpp)
target_compile_options(mobileNodeGUI_down PUBLIC -g)
target_link_libraries(mobileNodeGUI_down ${OpenCV_LIBS} w2rp cc_linkmonitor pthread boost_log boost_log_setup)
target_compile_definitions(mobileNodeGUI_down PUBLIC BOOST_LOG_DYN_LINK LOG_ON FILE_ON CONSOLE_ON)

# -------------------------------- Uplink -------------------------------- #
add_executable(mobileNode_up src/mobileNode/uplink/mobileNode_up.cpp)
target_compile_options(mobileNode_up PUBLIC -g)
target_link_libraries(mobileNode_up ${OpenCV_LIBS} w2rp cc_linkmonitor pthread boost_log boost_log_setup)
target_compile_definitions(mobileNode_up PUBLIC BOOST_LOG_DYN_LINK LOG_ON FILE_ON CONSOLE_ON)

# Enable automatic MOC (Meta-Object Compiler) generation for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

