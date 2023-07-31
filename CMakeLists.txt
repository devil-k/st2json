cmake_minimum_required(VERSION 3.22)
project(st2json C)

set(CMAKE_C_STANDARD 99)

include_directories(jansson)

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
        main.c
        )

add_executable(st2json ${SOURCES})
