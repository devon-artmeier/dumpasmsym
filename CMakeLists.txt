cmake_minimum_required(VERSION 3.16)
project(extract_asm_symbols LANGUAGES CXX)

add_executable(extract_asm_symbols
	"source/main.cpp"
	"source/helpers.cpp"
	"source/in_binary.cpp"
	"source/in_psyq.cpp"
	"source/in_vasm_lst.cpp"
	"source/in_vasm_vobj.cpp"
	"source/in_vlink_sym.cpp"
	"source/out_asm.cpp"
	"source/out_binary.cpp"
	"source/out_c.cpp"
	"source/symbols.cpp")