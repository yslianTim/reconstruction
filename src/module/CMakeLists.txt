# File dependencies
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/interface/include)
file(GLOB HEADER include/*.hpp)
file(GLOB SOURCE src/*.cpp)

# Add the library
add_library(Module SHARED ${SOURCE})

# Link the external libraries
target_link_libraries(Module PUBLIC ${EIGEN_LIBS})
target_link_libraries(Module PUBLIC ${ROOT_LIBS})
target_link_libraries(Module PUBLIC Interface)

# State that anybody linking to us needs to include the current source dir
target_include_directories(Module INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Install rules
install(TARGETS Module DESTINATION lib)
install(FILES ${HEADER} DESTINATION include)