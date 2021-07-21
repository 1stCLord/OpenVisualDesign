#pragma once
#include <string_view>
#include <filesystem>
#include <vector>

namespace ppparse
{
	class graph_node
	{
	public:
		graph_node(graph_node * const parent, const std::string_view& node_body);
	private:
		const std::string_view node_body;

		graph_node const * const parent;
		std::vector<graph_node *> children;
	};
}