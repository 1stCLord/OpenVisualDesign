#include "scope.h"
#include <algorithm>

namespace ppparse
{
	scope::scope(graph_node* const parent, const std::string_view& scope_preface, const std::string_view& scope_block) : 
		graph_node(parent, scope_block.substr(1,scope_block.size()-2), node_type::scope),
		scope_preface(scope_preface)
	{
		parse_expression(scope_preface);
		parse_block();
		parse_scope_preface();
	}

	const std::string_view scope::get_name() const
	{
		return scope_name;
	}

	scope::scope_type scope::get_scope_type() const
	{
		return m_scope_type;
	}

	void scope::parse_scope_preface()
	{
		using it_t = std::vector<graph_node*>::const_iterator;

		for(it_t it = preface_children.cbegin(); it != preface_children.cend(); it++)
		{
			graph_node const* node = *it;
			if (node->type == node_type::keyword && node->get_node_body() == "class")
			{
				m_scope_type = scope_type::class_scope;
				scope_name = parse_name(it+1, preface_children.cend());
				return;
			}
			if (node->type == node_type::function_parameters)
			{
				m_scope_type = scope_type::function_scope;
				std::reverse_iterator rit(it);
				scope_name = parse_name(rit, preface_children.crend());
				return;
			}
		}
	}

	void scope::add(graph_node* const child)
	{
		if (child->get_node_body().data() < get_node_body().data())
			preface_children.push_back(child);
		else
			children.push_back(child);
	}
}