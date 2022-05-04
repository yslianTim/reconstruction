
message(STATUS "Preparing MyProject...")
set(BINARY ${CMAKE_PROJECT_NAME})

# Add the library
add_subdirectory(interface)
add_subdirectory(module)

list(APPEND EXTRA_LIBS Interface Module)
list(APPEND EXTRA_LIBS ${EIGEN_LIBS} ${HDF5_LIBS} ${ROOT_LIBS} stdc++fs)


# Add MyProject executable
add_executable(${BINARY}_run main.cpp)


target_link_libraries(${BINARY}_run PUBLIC ${EXTRA_LIBS})


# Add the binary tree to the search path for include files
# so that we will find config.hpp
target_include_directories(${BINARY}_run PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${HDF5_INCLUDE_DIRS}"
                          )



install(TARGETS ${BINARY}_run DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.hpp" DESTINATION include)

#add_library(${BINARY}_lib STATIC main.cpp)