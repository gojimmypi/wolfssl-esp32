
#
# wolfssl server test
#
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWOLFSSL_USER_SETTINGS -DDEBUG_WOLFSSH")

set(COMPONENT_SRCS "app_smp_server_task.c" "test.h" "wifi_connect.c" "server.c")
set(COMPONENT_ADD_INCLUDEDIRS "." "./include")

register_component()
