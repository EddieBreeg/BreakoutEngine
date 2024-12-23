cmake_minimum_required(VERSION 3.20)
project(BZip2)

add_library(BZLib2 STATIC 
	blocksort.c
	bzlib.c
	compress.c
	crctable.c
	decompress.c
	huffman.c
	randtable.c
	bzlib.h
)
target_include_directories(BZLib2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(BZip2::BZLib ALIAS BZLib2)