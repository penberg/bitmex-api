add_executable(monitor monitor.cpp)

target_compile_features(monitor PUBLIC cxx_std_20)

target_compile_options(monitor PRIVATE -Wall -Wextra -pedantic -Werror)

target_link_libraries(monitor PRIVATE bitmex-api pthread crypto ssl)
