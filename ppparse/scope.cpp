#include "scope.h"

namespace ppparse
{
	scope::scope(graph_node* const parent, const std::string_view& scope_preface, const std::string_view& scope_block) : graph_node(parent, scope_block.substr(1,scope_block.size()-2))
	{
		parse_preface();
		parse_block();
	}

	void scope::parse_preface()
	{
		//check for 
		//attributes
		//templates
		//keywords
		//function declarations

	}
}