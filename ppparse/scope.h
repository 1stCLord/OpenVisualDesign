#pragma once 
#include "graph_node.h"
namespace ppparse
{
	class scope : public graph_node
	{
	public:
		scope(graph_node* const parent, const std::string_view& scope_preface, const std::string_view& scope_block);
	private:
		const std::string_view scope_preface;

		void parse_preface();
	};
}