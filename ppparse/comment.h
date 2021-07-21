#pragma once
#include "graph_node.h"

namespace ppparse
{
	class comment : public graph_node
	{
	public:
		comment(graph_node * const parent, const std::string_view& node_body) : graph_node(parent, node_body)
		{}
	};
}