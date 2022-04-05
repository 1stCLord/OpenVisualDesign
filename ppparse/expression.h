#pragma once 
#include "graph_node.h"

namespace ppparse
{
	class expression : public graph_node
	{
	public:
		expression(graph_node* const parent, const std::string_view& node_body, node_type type = node_type::expression);

		const std::string_view get_name() const override;

		std::string_view expression_name;
	};
}