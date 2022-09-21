#pragma once
#include <vector>
#include <string>
#include "Variable.h"

namespace ppparse
{
	class graph_node;
}

namespace OVD
{
	struct Callable
	{
		std::string classname;//todo
		std::string name;
		std::vector<Variable> parameters;
		std::string return_type;
		bool is_static;//todo

		Callable(ppparse::graph_node const* callable_node);
	private:
		ppparse::graph_node const* node;
	};
}