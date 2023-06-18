

cmake_minimum_required(VERSION 3.10)

project(nr_prach)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/common/")

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set(PRACH_UE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/generation/")
set(PRACH_gNB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/detection/")
set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common")

#sudo scp -r simde/ /usr/include/


  # set(CMAKE_C_FLAGS "-g3 -O0 -Wunused")
  # set(CMAKE_CXX_FLAGS "-g3 -O0 -Wunused")

  set(CMAKE_C_FLAGS "-O3")
  set(CMAKE_CXX_FLAGS "-O3")

set(C_FLAGS_PROCESSOR "${C_FLAGS_PROCESSOR} -DSIMDE_X86_AVX_NATIVE -DSIMDE_X86_AVX_NATIVE -DSIMDE_X86_F16C_NATIVE -DSIMDE_X86_FMA_NATIVE -DSIMDE_X86_GFNI_NATIVE -DSIMDE_X86_MMX_NATIVE -DSIMDE_X86_PCLMUL_NATIVE -DSIMDE_X86_SSE2_NATIVE -DSIMDE_X86_SSE3_NATIVE -DSIMDE_X86_SSE_NATIVE -DSIMDE_X86_XOP_HAVE_COM_ -DSIMDE_X86_XOP_NATIVE")
set(C_FLAGS_PROCESSOR "${C_FLAGS_PROCESSOR} -march=native")

set(CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} ${C_FLAGS_PROCESSOR} -std=gnu11 -funroll-loops")
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} ${C_FLAGS_PROCESSOR} -std=c++11")



add_library(oai_dfts SHARED ${COMMON_DIR}/oai_dfts_int.c ${COMMON_DIR}/dB_routines.c)
add_library(fftw3_dfts SHARED ${COMMON_DIR}/fftw.c)
# add_dependencies(fftw3_dfts fftw3)

add_executable(nr_prach ${SRC}/nr_prach.c
                        ${PRACH_UE_SRC}/prach_gen.c
                        ${PRACH_gNB_SRC}/prach_detect.c
                        ${COMMON_DIR}/write_output.c
                        )


target_link_libraries(nr_prach oai_dfts m fftw3_dfts fftw3f)