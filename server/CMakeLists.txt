cmake_minimum_required(VERSION 3.16)
project(leaserver)

set(CMAKE_CXX_STANDARD 14)

set(MYSQL_INC /usr/include/mysql)
set(MYSQL_LIB /usr/lib/mysql)
include_directories(${MYSQL_INC})
link_directories(${MYSQL_LIB})
link_libraries(mysqlclient)

include_directories(/usr/local/include/cjson)
link_directories(/usr/local/lib)
link_libraries(cjson)

set(LEADAO_INC include/)
set(LEADAO_LIB lib/)
include_directories(${LEADAO_INC})
link_directories(${LEADAO_LIB})
link_libraries(mesdb)

add_executable(leaserver
        main.cpp
        question.cpp
        include/question.h
        server.cpp
        include/server.h
        user.cpp
        include/user.h
        user_module.cpp mail_module.cpp contact_module.cpp)

target_link_libraries(leaserver mysqlclient)
target_link_libraries(leaserver mesdb)
find_package(OpenSSL REQUIRED)
target_link_libraries(leaserver OpenSSL::SSL)
target_link_libraries(leaserver cjson)
target_link_libraries(leaserver m)
target_link_libraries( leaserver -lpthread)