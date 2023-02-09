function(executable executable_name)
    set(options OPTIONAL WINDOWS)
    set(multiValueArgs SOURCES LIBS )
    cmake_parse_arguments(executable "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    add_executable(${executable_name} ${executable_SOURCES})
    if( ${executable_WINDOWS} AND MSVC)
        set_target_properties(${executable_name}  PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
    endif()
    target_link_libraries(${executable_name} ${executable_LIBS})
endfunction(executable)

dump_variable(
        Coin_LIBRARY
)

include_directories(
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GTK2_INCLUDE_DIRS}
)

add_subdirectory(components)
add_subdirectory(widgets)
