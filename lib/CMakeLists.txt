find_package(Boost COMPONENTS python310 REQUIRED)
find_package(PythonLibs 3.10 REQUIRED)

add_library(suffixtree SHARED)
target_include_directories(suffixtree PUBLIC ${PYTHON_INCLUDE_DIRS})

target_sources(suffixtree
    PUBLIC FILE_SET headers TYPE HEADERS BASE_DIRS include FILES
        include/suffix_tree.h
    PUBLIC
        src/suffix_tree.cpp
        src/bindings.cpp
)

target_link_libraries(suffixtree
   PUBLIC
        Boost::python
        ${PYTHON_LIBRARIES}
)