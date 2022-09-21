#pragma once
#include "Variable.h"
#include "graph_node.h"

namespace OVD
{
	std::string Variable::get_type_name() const 
	{
		return std::string(name_node->get_node_body()); 
	}
	
	std::string Variable::get_name() const 
	{ 
		return std::string(name_node->get_node_body()); 
	}

	Variable::Variable(ppparse::graph_node const* type_node, ppparse::graph_node const* name_node) :
		type_node(type_node), 
		name_node(name_node)
	{
	}
}