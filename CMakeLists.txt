cmake_minimum_required(VERSION 3.16)
project(LamportAuth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt first
find_package(Qt5 REQUIRED COMPONENTS Widgets Network Core)

add_executable(lamport-auth
    src/main.cpp
    src/auth/CryptoUtils.cpp
    src/auth/LamportAuth.cpp
    src/network/Client.cpp
    src/network/Server.cpp
    src/gui/MainWindow.cpp
    src/util/ConfigManager.cpp
)

# Headers in include/
target_include_directories(lamport-auth PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(lamport-auth PRIVATE
    cryptopp
    Qt5::Widgets
    Qt5::Network
    Qt5::Core
)

# Optional: binary in build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
