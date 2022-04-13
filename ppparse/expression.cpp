#include "expression.h"
#include "scope.h"

namespace ppparse
{
	expression::expression(graph_node* const parent, const std::string_view& node_body, node_type type) : graph_node(parent, node_body, type)
	{
		parse_expression(node_body);

		scope *parent_scope = static_cast<scope*>(ascend_until([](graph_node const* node)
			{
				return node->type == node_type::scope;
			}));
		//TODO if it's in class scope it's a declaration, the last set of symbols before the semi colon, assignment operator, or function parameter list is the name
		if (type == node_type::expression && parent_scope->get_scope_type() != scope::scope_type::function_scope)
		{
			//search backwards from the semi colon, find either an assignment operator or a function parameter list
			std::vector<graph_node*>::const_iterator name_end = children.cend(), name_begin = children.cbegin();
			for (std::vector<graph_node*>::const_reverse_iterator it = children.crbegin(); it != children.crend(); it++)
			{
				graph_node* node = *it;
				if (node->type == node_type::function_parameters)
				{
					name_end = it.base() - 1;
					break;
				}
			}

			for (std::vector<graph_node*>::const_reverse_iterator it = std::vector<graph_node*>::const_reverse_iterator(name_end); it != children.crend(); it++)
			{
				graph_node* node = *it;
				if (node->type != node_type::symbol && node->type != node_type::static_member)
				{
					name_begin = it.base();
					break;
				}
			}

			expression_name = parse_name(name_begin, name_end);
		}
		/*else
			expression_name = parse_name(children.cbegin(), children.cend());*/
	}

	const std::string_view expression::get_name() const
	{
		return expression_name;
	}
}