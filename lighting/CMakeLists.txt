set(src lighting.cpp)

add_executable(lighting ${src})

target_link_libraries(lighting common stb)
