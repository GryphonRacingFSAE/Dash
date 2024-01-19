set(CODE_FILES BusDump.cpp Dump.cpp)

set(HEADER_FILES Dump.hpp)

add_executable(BusDump ${CODE_FILES} ${HEADER_FILES})
set_target_properties(BusDump PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
target_link_libraries(BusDump PRIVATE fmt::fmt Threads::Threads CANLib)
target_include_directories(BusDump PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
