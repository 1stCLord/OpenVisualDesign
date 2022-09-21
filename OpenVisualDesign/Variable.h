#pragma once
#include <string>

namespace ppparse
{
	class graph_node;
}

namespace OVD
{
	struct Variable
	{
	public:
		Variable(ppparse::graph_node const* type_node, ppparse::graph_node const* name_node);

		std::string get_type_name() const;
		std::string get_name() const;
	private:
		ppparse::graph_node const* type_node;
		ppparse::graph_node const* name_node;
	};
}