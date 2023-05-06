cmake_minimum_required(VERSION 3.10)

project(nr_prach)


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(PRACH_UE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/generation/")
set(PRACH_gNB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/detection/")


add_executable(nr_prach ${SRC}/nr_prach.c
                        ${PRACH_UE_SRC}/prach_gen.c
                        ${PRACH_gNB_SRC}/prach_detect.c
                        )

# target_link_libraries(fftw3)