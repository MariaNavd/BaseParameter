#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

namespace p = boost::python;

class SuffixTree
{
    public:
        SuffixTree(std::vector<std::string> sequence);
        SuffixTree(p::list ns);
        SuffixTree(const SuffixTree&) = delete;
        SuffixTree(SuffixTree&&) = default;
        auto operator=(const SuffixTree&) -> SuffixTree& = delete;
        auto operator=(SuffixTree&&) -> SuffixTree& = default;
        ~SuffixTree();

        void build();
        std::map<std::vector<int>, std::vector<std::string>> get_edges();

    private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
};