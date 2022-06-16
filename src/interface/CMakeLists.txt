# File dependencies
include_directories(include)
include_directories(${HDF5_INCLUDE_DIRS})
file(GLOB HEADER include/*.hpp)
file(GLOB SOURCE src/*.cpp)

# Add the library
add_library(Interface SHARED ${SOURCE})

# Link the external libraries
target_link_libraries(Interface PUBLIC ${EIGEN_LIBS})
target_link_libraries(Interface PUBLIC ${ROOT_LIBS})

# State that anybody linking to us needs to include the current source dir
target_include_directories(Interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Install rules
install(TARGETS Interface DESTINATION lib)
install(FILES ${HEADER} DESTINATION include)