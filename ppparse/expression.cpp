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
		if (parent_scope->get_scope_type() == scope::scope_type::class_scope)
		{
			//search backwards from the semi colon, find either an assignment operator or a function parameter list
		}
		expression_name = parse_name(children.cbegin(), children.cend());
	}

	const std::string_view expression::get_name() const
	{
		return expression_name;
	}
}