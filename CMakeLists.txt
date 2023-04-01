cmake_minimum_required(VERSION 3.15)
project(multiclient_tcp_comm)

set(include_directories "include/tcpcom" "include")
set(linker_flags -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)

add_executable(tcpserver "src/server.cpp" "src/tcpcom/server.cpp" "src/keylogger.cpp")
add_executable(tcpclient "src/client.cpp" "src/tcpcom/client.cpp")
add_executable(filetransfer "src/filetransfer.cpp")

target_include_directories(tcpserver PUBLIC ${include_directories})
target_include_directories(tcpclient PUBLIC ${include_directories})

target_link_libraries(tcpserver PUBLIC "ws2_32" ${linker_flags})
target_link_libraries(tcpclient PUBLIC "ws2_32" ${linker_flags})
target_link_libraries(filetransfer PUBLIC "ws2_32" ${linker_flags})
