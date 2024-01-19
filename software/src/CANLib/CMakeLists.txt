set(CODE_FILES Interface/Interface.cpp Tools/tools.cpp)

set(HEADER_FILES Interface/Interface.hpp Interface/DBCInterface.hpp Tools/tools.hpp defs.hpp)

add_library(CANLib ${CODE_FILES} ${HEADER_FILES})
set_target_properties(CANLib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<0:>)
target_include_directories(CANLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Interface ${CMAKE_CURRENT_SOURCE_DIR}/Tools)
target_link_libraries(CANLib PUBLIC fmt::fmt dbcppp::dbcppp Threads::Threads)
