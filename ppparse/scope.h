#pragma once 
#include "graph_node.h"

namespace ppparse
{
	class source_file;
	class scope : public graph_node
	{
		friend source_file;
	public:
		scope(graph_node* const parent, const std::string_view& scope_preface, const std::string_view& scope_block);

		void add(graph_node* const child) override;
	private:
		const std::string_view scope_preface;
		std::vector<graph_node*> preface_children;
	};
}