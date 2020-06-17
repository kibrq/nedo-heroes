project(Heroes)
cmake_minimum_required(VERSION 3.16)

set(EXE heroes)
set(INC include)
set(SRC src)


set_property(GLOBAL PROPERTY ALL_SRC)

function(add_source)
	get_property(tmp GLOBAL PROPERTY ALL_SRC)
	list(APPEND tmp ${ARGV})
	set_property(GLOBAL PROPERTY ALL_SRC ${tmp})
endfunction(add_source)

add_subdirectory(src)

get_property(ALL_SOURCES GLOBAL PROPERTY ALL_SRC)
add_executable(${EXE} ${ALL_SOURCES})


target_link_libraries(${EXE}
	sfml-graphics
	sfml-window
	sfml-system)

