include_directories(${CMAKE_SOURCE_DIR}/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/module/include)

set(BINARY ${CMAKE_PROJECT_NAME}_test)
file(GLOB HEADER *.hpp)
file(GLOB SOURCE *.cpp)
set(SOURCES ${SOURCE} ${HEADER})

add_executable(${BINARY} ${SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC Module Interface)
target_link_libraries(${BINARY} PUBLIC ${EIGEN_LIBS})
target_link_libraries(${BINARY} PUBLIC ${HDF5_LIBS})
target_link_libraries(${BINARY} PUBLIC ${ROOT_LIBS})
target_link_libraries(${BINARY} PUBLIC ${EXTRA_LIBS} gtest_main gmock_main stdc++fs)

target_include_directories(${BINARY} PUBLIC "${HDF5_INCLUDE_DIRS}")