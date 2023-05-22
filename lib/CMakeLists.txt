find_package(Boost COMPONENTS python310 REQUIRED)
find_package(PythonLibs 3.10 REQUIRED)

add_library(suffixtree SHARED)
target_include_directories(suffixtree
    PUBLIC
        ${PYTHON_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)

target_sources(suffixtree
    PRIVATE
        src/suffix_tree.cpp
        src/bindings.cpp
)

target_link_libraries(suffixtree
   PUBLIC
        Boost::python
        ${PYTHON_LIBRARIES}
)
