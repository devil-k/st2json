cmake_minimum_required(VERSION 3.16)
project(st2json C)

set(CMAKE_C_STANDARD 99)

include_directories(jansson)
include_directories(base64)

set(SOURCES
        jansson/dump.c
        jansson/error.c
        jansson/hashtable.c
        jansson/load.c
        jansson/memory.c
        jansson/pack_unpack.c
        jansson/path.c
        jansson/strbuffer.c
        jansson/strconv.c
        jansson/utf.c
        jansson/value.c

        base64/base64.c

        main.c
        )

add_executable(st2json ${SOURCES})
