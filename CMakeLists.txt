cmake_minimum_required(VERSION 3.16)
project(dumpasmsym LANGUAGES CXX)

add_executable(dumpasmsym
	"src/main.cpp"
	"src/helpers.cpp"
	"src/in_binary.cpp"
	"src/in_psyq.cpp"
	"src/in_vasm_lst.cpp"
	"src/in_vasm_vobj.cpp"
	"src/in_vlink_sym.cpp"
	"src/out_asm.cpp"
	"src/out_binary.cpp"
	"src/out_c.cpp"
	"src/symbols.cpp")

install(TARGETS dumpasmsym)