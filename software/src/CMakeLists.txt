add_subdirectory(Interfaces)

if(${BUILD_BACKEND})
    add_subdirectory(CANLib)
    add_subdirectory(BusDump)
endif()

if(${BUILD_FRONTEND})
    add_subdirectory(Dash)
endif()
