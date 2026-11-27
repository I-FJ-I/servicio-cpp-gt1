cmake_minimum_required(VERSION 3.21)

project(servicio-cpp-gt1 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilación" FORCE)
endif()

# Define la opción BUILD_TESTING (ON por defecto) y habilita CTest
include(CTest)
include(FetchContent)

# ---- Dependencias (versión fijada + hash SHA-256 para builds reproducibles) ----

# Servidor HTTP, solo cabeceras. Sin TLS ni compresión: no los necesitamos.
set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_ZLIB_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  httplib
  URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.20.1.tar.gz
  URL_HASH SHA256=b74b1c2c150be2841eba80192f64d93e9a6711985b3ae8aaa1a9cec4863d1dd9
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(httplib)

# ---- Código del microservicio ----

# Algoritmos: sin dependencias, para poder testearlos de forma aislada
add_library(algorithms STATIC src/algorithms/mergesort.cpp)
target_include_directories(algorithms PUBLIC src)

# Rutas de la API REST: separadas de main() para poder testearlas en proceso
add_library(api STATIC src/api/server.cpp)
target_include_directories(api PUBLIC src)
target_link_libraries(api PUBLIC algorithms httplib::httplib)

add_executable(servicio-cpp-gt1 src/main.cpp)
target_link_libraries(servicio-cpp-gt1 PRIVATE api)

foreach(target algorithms api servicio-cpp-gt1)
  target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()

# ---- Tests ----

if(BUILD_TESTING)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    URL_HASH SHA256=7b42b4d6ed48810c5362c265a17faebe90dc2373c885e5216439d37927f02926
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_MakeAvailable(googletest)

  add_subdirectory(tests)
endif()
