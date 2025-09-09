# # cmake_minimum_required(VERSION 3.16)
# # project(LamportAuth LANGUAGES CXX)

# # set(CMAKE_CXX_STANDARD 17)
# # set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # set(CMAKE_AUTOUIC ON)
# # set(CMAKE_AUTOMOC ON)
# # set(CMAKE_AUTORCC ON)

# # # Find Qt first
# # find_package(Qt5 REQUIRED COMPONENTS Widgets Network Core)

# # add_executable(lamport-auth
# #     src/main.cpp
# #     src/auth/CryptoUtils.cpp
# #     src/auth/LamportAuth.cpp
# #     src/network/Client.cpp
# #     src/network/Server.cpp
# #     src/gui/MainWindow.cpp
# #     src/util/ConfigManager.cpp
# #     include/Client.hpp
# #     include/Server.hpp
# #     include/MainWindow.hpp
# # )


# # # Headers in include/
# # # This is the ONLY line needed to handle your headers.
# # target_include_directories(lamport-auth PRIVATE ${CMAKE_SOURCE_DIR}/include)

# # # Link libraries
# # target_link_libraries(lamport-auth PRIVATE
# #     cryptopp
# #     Qt5::Widgets
# #     Qt5::Network
# #     Qt5::Core
# # )

# # # Optional: binary in build/bin/

# cmake_minimum_required(VERSION 3.16)
# project(LamportAuth LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# # Find Qt
# find_package(Qt5 REQUIRED COMPONENTS Widgets Network Core)

# # ------------------------------
# # Main GUI executable
# # ------------------------------
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

# target_include_directories(lamport-auth PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target_link_libraries(lamport-auth PRIVATE
#     cryptopp
#     Qt5::Widgets
#     Qt5::Network
#     Qt5::Core
# )

# # ------------------------------
# # Server test executable
# # ------------------------------
# add_executable(lamport-server
#     src/server_main.cpp
#     src/auth/CryptoUtils.cpp
#     src/auth/LamportAuth.cpp
#     src/network/Server.cpp
#     include/Server.hpp
#     src/util/ConfigManager.cpp
# )

# target_include_directories(lamport-server PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target_link_libraries(lamport-server PRIVATE
#     cryptopp
#     Qt5::Network
#     Qt5::Core
# )

# # ------------------------------
# # Client test executable
# # ------------------------------
# add_executable(lamport-client
#     src/client_main.cpp
#     src/auth/CryptoUtils.cpp
#     src/auth/LamportAuth.cpp
#     src/network/Client.cpp
#     include/Client.hpp
#     src/util/ConfigManager.cpp
# )

# target_include_directories(lamport-client PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target_link_libraries(lamport-client PRIVATE
#     cryptopp
#     Qt5::Network
#     Qt5::Core
# )

cmake_minimum_required(VERSION 3.16)
project(LamportAuth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic tools for processing UI files, MOC, and RCC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt5 package and specify the components needed for all targets
find_package(Qt5 REQUIRED COMPONENTS Widgets Network Core)

# --- Common Source Files ---
# Define common source files that will be used by multiple executables
set(COMMON_AUTH_SOURCES
    src/auth/CryptoUtils.cpp
    src/auth/LamportAuth.cpp
    include/CryptoUtils.hpp # Include header for AUTOCONFIG
    include/LamportAuth.hpp # Include header for AUTOCONFIG
)

set(COMMON_UTIL_SOURCES
    src/util/ConfigManager.cpp
    include/ConfigManager.hpp # Include header for AUTOCONFIG
)

# --- Lamport Authenticator GUI executable ---
add_executable(lamport-auth-gui
    src/main.cpp # Your GUI main function
    src/gui/MainWindow.cpp
    src/gui/mainwindow.ui # The .ui file
    include/MainWindow.hpp # The header for GUI
    src/network/Client.cpp
    include/Client.hpp # The header for Client
    src/network/Server.cpp
    include/Server.hpp # The header for Server
    ${COMMON_AUTH_SOURCES}
    ${COMMON_UTIL_SOURCES}
)

# Set include directories for the GUI app
target_include_directories(lamport-auth-gui PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link Qt5 libraries and Crypto++ for the GUI app
target_link_libraries(lamport-auth-gui PRIVATE
    cryptopp
    Qt5::Widgets
    Qt5::Network
    Qt5::Core
)

# --- Lamport Server Console executable ---
add_executable(lamport-server-console
    src/server_main.cpp # Your console server main
    src/network/Server.cpp
    include/Server.hpp # The header for Server
    ${COMMON_AUTH_SOURCES}
    ${COMMON_UTIL_SOURCES}
)

target_include_directories(lamport-server-console PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lamport-server-console PRIVATE
    cryptopp
    Qt5::Network
    Qt5::Core
)

# --- Lamport Client Console executable ---
add_executable(lamport-client-console
    src/client_main.cpp # Your console client main
    src/network/Client.cpp
    include/Client.hpp # The header for Client
    ${COMMON_AUTH_SOURCES}
    ${COMMON_UTIL_SOURCES}
)

target_include_directories(lamport-client-console PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(lamport-client-console PRIVATE
    cryptopp
    Qt5::Network
    Qt5::Core
)