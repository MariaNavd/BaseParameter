#include "suffix_tree.h"

class Node
{
    public:
        friend SuffixTree;
        Node(std::shared_ptr<Node> link, int start, int *end, int ind = -1)
            : suffix_link(link), start(start), end(end), suffix_index(ind)
        {
        }

    private:
        std::map<std::string, std::shared_ptr<Node>> children;
        std::shared_ptr<Node> suffix_link;
        int start;
        int* end;
        int suffix_index;

        int num = -1;
};

class SuffixTree::Impl
{
    public:
        Impl(std::vector<std::string> sequence);
        Impl(p::list ns);

        void build();
        std::map<std::vector<int>, std::vector<std::string>> get_edges();

    private:
        void extend(int pos);
        auto edge_length(std::shared_ptr<Node>& node) -> int;
        void breadth_first_search();

        std::shared_ptr<Node> root = std::make_shared<Node>(nullptr, -1, new int(-1));
        std::shared_ptr<Node> last_created_internal_node;

        std::vector<std::string> sequence;
        const char* termination_symbol = "$";

        std::shared_ptr<Node> active_node;
        int active_edge = -1;
        int active_length = 0;
        int remaining_suffix_count = 0;
        int leaf_end = -1;

        std::vector<std::shared_ptr<Node>> nodes_list;
};

SuffixTree::SuffixTree(std::vector<std::string> sequence)
    : pimpl(std::make_unique<Impl>(std::move(sequence)))
{
}

SuffixTree::Impl::Impl(std::vector<std::string> sequence) : sequence(sequence)
{
    sequence.push_back(termination_symbol);
    build();
}

SuffixTree::SuffixTree(p::list ns)
    : pimpl(std::make_unique<Impl>(ns))
{
}

SuffixTree::Impl::Impl(p::list ns)
{
    for (int i = 0; i < len(ns); ++i)
    {
        sequence.push_back(p::extract<std::string>(ns[i]));
    }
    sequence.push_back(termination_symbol);
    build();
}

SuffixTree::~SuffixTree() noexcept = default;

void SuffixTree::build()
{
    pimpl->build();
}

void SuffixTree::Impl::build()
{
    active_node = root;
    for (size_t i = 0; i < sequence.size(); i++)
    {
        extend(i);
    }
    breadth_first_search();
}

int SuffixTree::Impl::edge_length(std::shared_ptr<Node>& node)
{
    return *(node->end) - (node->start) + 1;
}

void SuffixTree::Impl::extend(int phase)
{
    last_created_internal_node = nullptr;
    leaf_end++;
    remaining_suffix_count++;

    while (remaining_suffix_count > 0)
    {
        if (active_length == 0)
        {
            active_edge = phase;
        }

        auto find = active_node->children.find(sequence[active_edge]);
        
        if (find == active_node->children.end())
        {
            active_node->children.insert(std::make_pair(
                sequence[active_edge],
                std::make_shared<Node>(root, phase, &leaf_end, phase - remaining_suffix_count + 1)));

            if (last_created_internal_node != nullptr)
            {
                last_created_internal_node->suffix_link = active_node;
                last_created_internal_node = nullptr;
            }
        }
        else
        {
            auto next = find->second;
            int edge_size = edge_length(next);

            if (active_length >= edge_size)
            {
                active_edge += edge_size;
                active_length -= edge_size;
                active_node = next;
                continue;
            }

            if (sequence[next->start + active_length] == sequence[phase])
            {
                if (last_created_internal_node != nullptr && active_node != root)
                {
                    last_created_internal_node->suffix_link = active_node;
                }
                active_length++;
                break;
            }

            std::shared_ptr<Node> split = std::make_shared<Node>(root, next->start, new int(next->start + active_length - 1));
            active_node->children[sequence[active_edge]] = split;
            next->start += active_length;
            split->children.insert(std::make_pair(sequence[phase], std::make_shared<Node>(root, phase, &leaf_end, phase - remaining_suffix_count + 1)));
            split->children.insert(std::make_pair(sequence[next->start], next));

            if (last_created_internal_node != nullptr)
            {
                last_created_internal_node->suffix_link = split;
            }
            last_created_internal_node = split;
        }

        remaining_suffix_count--;

        if (active_node == root && active_length > 0)
        {
            active_length--;
            active_edge++;
        }
        else if (active_node != root)
        {
            active_node = active_node->suffix_link;
        }
    }
}

void SuffixTree::Impl::breadth_first_search()
{
    std::vector<std::shared_ptr<Node>> not_visited;

    int count = 1;
    root->num = 0;
    nodes_list.push_back(root);

    for (auto& chld : root->children)
    {
        not_visited.push_back(chld.second);
    }

    auto current_node = not_visited.front();

    while (not_visited.size() != 0)
    {
        current_node->num = count;
        nodes_list.push_back(current_node);
        not_visited.erase(not_visited.begin());
        
        for (auto& chld : current_node->children)
        {
            not_visited.push_back(chld.second);
        }

        current_node = not_visited.front();
        count++;
    }
}

std::map<std::vector<int>, std::vector<std::string>> SuffixTree::get_edges()
{
    return pimpl->get_edges();
}

std::map<std::vector<int>, std::vector<std::string>> SuffixTree::Impl::get_edges()
{
    std::map<std::vector<int>, std::vector<std::string>> edges;

    for (auto& node : nodes_list)
    {
        for (auto& chld : node->children)
        {
            auto first = sequence.cbegin() + chld.second->start;
            auto last = sequence.cbegin() + *(chld.second->end) + 1;
        
            std::vector<std::string> vec(first, last);
            std::vector<int> vec1 {node->num, chld.second->num};

            edges.insert(std::make_pair(vec1, vec));
        }
    }

    return edges;
}