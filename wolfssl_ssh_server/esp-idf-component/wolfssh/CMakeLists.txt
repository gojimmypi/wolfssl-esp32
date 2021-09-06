#
# cmake for wolfssh
#
cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWOLFSSL_USER_SETTINGS -Wno-error")

set(CMAKE_CURRENT_SOURCE_DIR ".")
set(WOLFSSH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_PATH ${WOLFSSH_ROOT})

set(COMPONENT_SRCDIRS "./src/"
                      )

set(COMPONENT_REQUIRES lwip)

set(COMPONENT_ADD_INCLUDEDIRS
    "."
    "./include"
    "../freertos/include/freertos"
    "../wolfssl"
    "${WOLFSSH_ROOT}"
    )

register_component()
