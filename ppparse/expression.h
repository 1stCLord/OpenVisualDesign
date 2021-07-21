#pragma once 
#include "graph_node.h"

namespace ppparse
{
	class expression : public graph_node
	{
	public:
		expression(graph_node* const parent, const std::string_view& node_body) : graph_node(parent, node_body)
		{}
	};
}