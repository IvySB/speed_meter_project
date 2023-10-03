cmake_minimum_required(VERSION 3.22)

project(teamwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UARTCOM ON)

set(SERVER_DIR server/desktop)
set(CLIENT_DIR client/desktop)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Widgets SerialPort Multimedia)

set(CLIENT_HEADERS shared/setting.h ${CLIENT_DIR}/include/windows.h ${CLIENT_DIR}/include/canvas.h ${CLIENT_DIR}/include/comservice.h)
set(CLIENT_SOURCES ${CLIENT_DIR}/main.cpp ${CLIENT_DIR}/src/windows.cpp ${CLIENT_DIR}/src/canvas.cpp ${CLIENT_DIR}/src/comservice.cpp)
set(CLIENT_LIBRARIES Qt6::Core Qt6::Widgets Qt6::Multimedia)

set(SERVER_HEADERS shared/setting.h ${SERVER_DIR}/include/window.h ${SERVER_DIR}/include/comservices.h ${SERVER_DIR}/include/comservices.h)
set(SERVER_SOURCES ${SERVER_DIR}/main.cpp ${SERVER_DIR}/src/window.cpp ${SERVER_DIR}/src/comservices.cpp ${SERVER_DIR}/src/comservices.cpp)
set(SERVER_LIBRARIES Qt6::Core Qt6::Widgets)

if(${UARTCOM} MATCHES ON)
    add_compile_definitions(UARTCOM)

    set(CLIENT_HEADERS ${CLIENT_HEADERS} ${CLIENT_DIR}/include/uartservice.h)
    set(CLIENT_SOURCES ${CLIENT_SOURCES} ${CLIENT_DIR}/src/uartservice.cpp)
    set(CLIENT_LIBRARIES ${CLIENT_LIBRARIES} Qt6::SerialPort)

    set(SERVER_HEADERS ${SERVER_HEADERS} ${SERVER_DIR}/include/uartservices.h)
    set(SERVER_SOURCES ${SERVER_SOURCES} ${SERVER_DIR}/src/uartservices.cpp)
    set(SERVER_LIBRARIES ${SERVER_LIBRARIES} Qt6::SerialPort)

else()

    set(CLIENT_HEADERS ${CLIENT_HEADERS} ${CLIENT_DIR}/include/tcpclient.h)
    set(CLIENT_SOURCES ${CLIENT_SOURCES} ${CLIENT_DIR}/src/tcpclient.cpp)

    set(SERVER_HEADERS ${SERVER_HEADERS} ${SERVER_DIR}/include/tcpserver.h)
    set(SERVER_SOURCES ${SERVER_SOURCES} ${SERVER_DIR}/src/tcpserver.cpp)
endif()    

add_executable(server ${SERVER_HEADERS} ${SERVER_SOURCES})
target_link_libraries(server PUBLIC ${SERVER_LIBRARIES})
target_include_directories(server PUBLIC shared ${SERVER_DIR}/include)
    
add_executable(client ${CLIENT_HEADERS} ${CLIENT_SOURCES})
target_link_libraries(client PUBLIC ${CLIENT_LIBRARIES})
target_include_directories(client PUBLIC shared ${CLIENT_DIR}/include)

add_custom_target(upload_client
    COMMAND platformio run -t upload
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/client/esp32
    COMMENT "Uploading client firmware to microcontroller..."
)

add_dependencies(upload_client client)

add_custom_target(upload_server
    COMMAND platformio run -t upload
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/server/esp32
    COMMENT "Uploading server firmware to microcontroller..."
)

add_dependencies(upload_server server)
    
    #done with server part of the cmake list!