#pragma once
#include "graph_node.h"

namespace ppparse
{
	class preprocessor_directive : public graph_node
	{
	public:
		preprocessor_directive(graph_node * const parent, const std::string_view& node_body) : graph_node(parent, node_body)
		{}
	};
}