cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(antpov LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_MODULE_STD 1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

# Locate the NVRT distribution.  Search the SDK first, then look in the system.
set(OptiX_INSTALL_DIR "${CMAKE_SOURCE_DIR}/../" CACHE PATH "Path to OptiX installed location.")

# Search for the OptiX libraries and include files.
find_package(OptiX REQUIRED)

include_directories(BEFORE ${PROJECT_SOURCE_DIR})
include_directories(BEFORE "${PROJECT_SOURCE_DIR}/maths")
include_directories(BEFORE "/usr/local/include/abi_ex")
include_directories(BEFORE "/usr/local/include/compound-ray")
include_directories("${OptiX_INCLUDE}")
#
# Module groups
#
set(SM_CONSTEXPR_MATH_MODULES
  ${PROJECT_SOURCE_DIR}/maths/sm/mathconst
  ${PROJECT_SOURCE_DIR}/maths/sm/constexpr_math
)
set(SM_POLYSOLVE_MODULES
  ${SM_CONSTEXPR_MATH_MODULES}
  ${PROJECT_SOURCE_DIR}/maths/sm/polysolve
)
set(SM_BESSEL_I0_MODULES
  ${SM_POLYSOLVE_MODULES}
  ${PROJECT_SOURCE_DIR}/maths/sm/bessel_i0
)
set(SM_RANDOM_MODULES
  ${SM_BESSEL_I0_MODULES}
  ${PROJECT_SOURCE_DIR}/maths/sm/random
)
set(SM_RANGE_MODULES
  ${SM_CONSTEXPR_MATH_MODULES}
  ${PROJECT_SOURCE_DIR}/maths/sm/trait_tests
  ${PROJECT_SOURCE_DIR}/maths/sm/range
)
set(SM_VEC_MODULES
  ${SM_RANGE_MODULES}
  ${SM_RANDOM_MODULES}
  ${PROJECT_SOURCE_DIR}/maths/sm/vec
)
set(SM_VVEC_MODULES
  ${SM_RANGE_MODULES}
  ${SM_RANDOM_MODULES}
  ${PROJECT_SOURCE_DIR}/maths/sm/vvec
)

# Macro similar to that used in antpov
function(add_modules_exe target_name_base target_name_var)
  set( target_name ${target_name_base} )
  set( ${target_name_var} ${target_name} PARENT_SCOPE )
  # Parse the rest of the args, which are all multiValueArgs
  set(multiValueArgs SOURCES MODULES OPTIONS CMAKE_OPTIONS)
  cmake_parse_arguments(PARSE_ARGV 0 arg
    "${options}" "${oneValueArgs}" "${multiValueArgs}"
  )
  add_executable(${target_name} ${arg_SOURCES} ${generated_files} ${arg_CMAKE_OPTIONS})
  target_sources(${target_name} PUBLIC FILE_SET CXX_MODULES BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES ${arg_MODULES}
  )
  if(USING_GNU_CXX)
    target_link_libraries( ${target_name} m ) # Explicitly link against math library
  endif()
endfunction()

set_source_files_properties (${SM_VVEC_MODULES} PROPERTIES LANGUAGE CXX)
add_modules_exe (abi_prog target_name SOURCES abi_prog.cpp MODULES ${SM_VVEC_MODULES} OPTIONS -rdc true)
target_link_libraries(abi_prog /usr/local/lib/libabi_ex.so /usr/local/lib/libEyeRenderer3.so)
