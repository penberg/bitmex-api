set(LIBRARY_NAME bitmex-api)

set(HEADERS_PUBLIC include/bitmex/bitmex.hpp)

#set(SOURCES src/simdjson.cpp)

#set(HEADERS_PRIVATE src/simdjson.h)

add_library(${LIBRARY_NAME} ${HEADERS_PUBLIC} ${SOURCES} ${HEADERS_PRIVATE})

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_20)

target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)

target_include_directories(${LIBRARY_NAME} PUBLIC include)

#target_link_libraries(${LIBRARY_NAME} PUBLIC rapidjson)
