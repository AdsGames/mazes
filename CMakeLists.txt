cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

project(Mazes)

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  asw
  GIT_REPOSITORY https://github.com/adsgames/asw.git
  GIT_TAG        v0.5.1
)
FetchContent_MakeAvailable(asw)

# Link Libs
if(MINGW)
  target_link_libraries(${PROJECT_NAME} -lmingw32)
endif(MINGW)

target_link_libraries(
  ${PROJECT_NAME} 
  asw
  nlohmann_json::nlohmann_json
)

# Emscripten support
if(EMSCRIPTEN)
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  target_link_libraries(
    ${PROJECT_NAME} 
    -sWASM=1 
    -sALLOW_MEMORY_GROWTH=1 
    -sMAXIMUM_MEMORY=1gb
  )

  set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES 
    LINK_FLAGS
    "--preload-file ${CMAKE_CURRENT_LIST_DIR}/assets/data@/data --preload-file ${CMAKE_CURRENT_LIST_DIR}/assets/sounds@/sounds --preload-file ${CMAKE_CURRENT_LIST_DIR}/assets/fonts@/fonts --preload-file ${CMAKE_CURRENT_LIST_DIR}/assets/images@/images --use-preload-plugins --shell-file ${CMAKE_CURRENT_LIST_DIR}/index.html"
  )
endif(EMSCRIPTEN)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/data DESTINATION ${CMAKE_BINARY_DIR}/build)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/fonts DESTINATION ${CMAKE_BINARY_DIR}/build)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/images DESTINATION ${CMAKE_BINARY_DIR}/build)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/sounds DESTINATION ${CMAKE_BINARY_DIR}/build)