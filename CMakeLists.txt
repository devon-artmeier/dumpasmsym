cmake_minimum_required(VERSION 3.16)
project(extract-asm-symbols LANGUAGES CXX)

add_executable(extract-asm-symbols
	"source/main.cpp"
	"source/helpers.cpp"
	"source/psyq.cpp"
	"source/symbols.cpp"
	"source/vasm_vobj.cpp")