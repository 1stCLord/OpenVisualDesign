#include "Callable.h"
#include "graph_node.h"
#include <algorithm>

namespace OVD
{
	Callable::Callable(ppparse::graph_node const* callable_node) : node(callable_node)
	{
		using namespace ppparse;
		using namespace std::ranges;
		using graph_node_it = std::vector<graph_node*>::const_iterator;
		name = std::string(callable_node->get_name());


		auto type_projection = [](graph_node const* node) {return node->type; };
		const std::vector<graph_node*>& children = callable_node->get_children();
		graph_node_it function_parameter_iterator = find(children, node_type::function_parameters, type_projection);
		if (function_parameter_iterator != children.cend())
		{
			graph_node const* function_parameters = *function_parameter_iterator;
			std::vector<std::span<graph_node* const>> function_parameters_members = function_parameters->children_as_parameter_list_members();
			for (const std::span<graph_node* const> &function_parameter : function_parameters_members)
			{
				graph_node* const type_node = function_parameter.back();
				graph_node* const name_node = function_parameter.back();
				parameters.push_back(Variable(type_node, name_node));
				//parameters.push_back(Variable{ .name = std::string(name_node->get_node_body()), .type_name = type_node->get_});
			}
		}
	}
}