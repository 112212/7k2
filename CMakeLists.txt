cmake_minimum_required(VERSION 2.8)
project(7K2)
set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake/modules)

if(${MSVC})
  if(NOT ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ""
        CACHE PATH "Path to 3rd party libraries")
  endif()
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /DWIN32_LEAN_AND_MEAN)
  find_path(HAVE_STDINT_H stdint.h)
  if(NOT ${HAVE_STDINT_H})
    include_directories(${CMAKE_HOME_DIRECTORY}/cmake/include)
  endif()
endif()
add_definitions(-DUSE_OPENAL)

# if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	# set(DEBUG 1)
# endif()

if(DEBUG EQUAL 1)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()


find_package(ENet REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(SDL2 REQUIRED)

add_subdirectory(src)
add_subdirectory(src/imgfun)

include_directories(/usr/include/SDL2)
