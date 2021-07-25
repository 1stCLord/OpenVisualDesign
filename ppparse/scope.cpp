#include "scope.h"

namespace ppparse
{
	scope::scope(graph_node* const parent, const std::string_view& scope_preface, const std::string_view& scope_block) : 
		graph_node(parent, scope_block.substr(1,scope_block.size()-2)),
		scope_preface(scope_preface)
	{
		parse_preface();
		parse_block();
	}

	void scope::parse_preface()
	{
		size_t position = 0;
		do
		{
			size_t initial_position = position;
			comments_and_preprocessor_directives(position, scope_preface);
			attributes(position, scope_preface);
			template_parameters(position, scope_preface);
			keywords(position, scope_preface);
			function_parameters(position, scope_preface);
			if (position == initial_position)
				consume_symbol(position, scope_preface);
		} while (!is_end(position, scope_preface));

	}
}