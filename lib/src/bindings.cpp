#include "suffix_tree.h"

boost::python::dict get_edges_info(PyObject* pyobj)
{
    SuffixTree *suffix_tree = boost::python::extract<SuffixTree*>(pyobj);
    std::map<std::vector<int>, std::vector<std::string>> edges = suffix_tree->get_edges();

    boost::python::dict d;

    for (auto tmp : edges)
    {
        p::object get_iter = p::iterator<std::vector<std::string> >();
        p::object iter = get_iter(tmp.second);
        p::list l(iter);

        p::object get_iter1 = p::iterator<std::vector<int> >();
        p::object iter1 = get_iter1(tmp.first);
        p::tuple l1(iter1);
        d[l1] = l;
	}
    return d;
}

BOOST_PYTHON_MODULE(libsuffixtree)
{
    p::class_<std::vector<int>>("IntVec")
        .def(p::vector_indexing_suite<std::vector<int>>());
    p::class_<std::vector<std::string> >("StringVec")
        .def(p::vector_indexing_suite<std::vector<std::string>>());

    p::class_<SuffixTree, boost::noncopyable>("SuffixTree", p::init<p::list>())
        .def("get_edges", &get_edges_info, p::args( "pyobj" ) );
}