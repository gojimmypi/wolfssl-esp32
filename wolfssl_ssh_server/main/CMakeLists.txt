
#
# wolfssl server test
#
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWOLFSSL_USER_SETTINGS")

set(COMPONENT_SRCS "app_smp_server_task.c" "wifi_connect.c")
set(COMPONENT_ADD_INCLUDEDIRS "." "./include")

register_component()
