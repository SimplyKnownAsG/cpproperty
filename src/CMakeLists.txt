# find source files
file(GLOB_RECURSE CPPROPERTY_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpproperty/*.cpp)

add_library(cpproperty INTERFACE)
target_compile_features(cpproperty INTERFACE cxx_std_11)


target_include_directories(cpproperty
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
