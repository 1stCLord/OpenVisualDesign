#include "graph_node.h"

namespace ppparse
{
	graph_node::graph_node(graph_node * const parent, const std::string_view& node_body) :
		parent(parent),
		node_body(node_body)
	{
		if(parent != nullptr)
			parent->children.push_back(this);
	}
}