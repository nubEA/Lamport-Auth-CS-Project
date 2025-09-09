# cmake_minimum_required(VERSION 3.16)
# project(LamportAuth LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# # Find Qt first
# find_package(Qt5 REQUIRED COMPONENTS Widgets Network Core)

# add_executable(lamport-auth
#     src/main.cpp
#     src/auth/CryptoUtils.cpp
#     src/auth/LamportAuth.cpp
#     src/network/Client.cpp
#     src/network/Server.cpp
#     src/gui/MainWindow.cpp
#     src/util/ConfigManager.cpp
#     include/Client.hpp
#     include/Server.hpp
#     include/MainWindow.hpp
# )


# # Headers in include/
# # This is the ONLY line needed to handle your headers.
# target_include_directories(lamport-auth PRIVATE ${CMAKE_SOURCE_DIR}/include)

# # Link libraries
# target_link_libraries(lamport-auth PRIVATE
#     cryptopp
#     Qt5::Widgets
#     Qt5::Network
#     Qt5::Core
# )

# # Optional: binary in build/bin/

cmake_minimum_required(VERSION 3.16)
project(LamportAuth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt
find_package(Qt5 REQUIRED COMPONENTS Widgets Network Core)

# ------------------------------
# Main GUI executable
# ------------------------------
add_executable(lamport-auth
    src/main.cpp
    src/auth/CryptoUtils.cpp
    src/auth/LamportAuth.cpp
    src/network/Client.cpp
    src/network/Server.cpp
    src/gui/MainWindow.cpp
    src/util/ConfigManager.cpp
    include/Client.hpp
    include/Server.hpp
    include/MainWindow.hpp
)

target_include_directories(lamport-auth PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lamport-auth PRIVATE
    cryptopp
    Qt5::Widgets
    Qt5::Network
    Qt5::Core
)

# ------------------------------
# Server test executable
# ------------------------------
add_executable(lamport-server
    src/server_main.cpp
    src/auth/CryptoUtils.cpp
    src/auth/LamportAuth.cpp
    src/network/Server.cpp
    include/Server.hpp
    src/util/ConfigManager.cpp
)

target_include_directories(lamport-server PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lamport-server PRIVATE
    cryptopp
    Qt5::Network
    Qt5::Core
)

# ------------------------------
# Client test executable
# ------------------------------
add_executable(lamport-client
    src/client_main.cpp
    src/auth/CryptoUtils.cpp
    src/auth/LamportAuth.cpp
    src/network/Client.cpp
    include/Client.hpp
    src/util/ConfigManager.cpp
)

target_include_directories(lamport-client PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lamport-client PRIVATE
    cryptopp
    Qt5::Network
    Qt5::Core
)
