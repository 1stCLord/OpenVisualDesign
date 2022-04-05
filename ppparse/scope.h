#pragma once 
#include "graph_node.h"

namespace ppparse
{
	class source_file;
	class scope : public graph_node
	{
		friend source_file;
	public:
		enum class scope_type : uint8_t
		{
			class_scope,
			function_scope,
			undefined_scope
		};

		scope(graph_node* const parent, const std::string_view& scope_preface, const std::string_view& scope_block);

		const std::string_view get_name() const override;
		scope_type get_scope_type() const;

		void add(graph_node* const child) override;
	private:
		const std::string_view scope_preface;
		std::vector<graph_node*> preface_children;

		void parse_scope_preface();
		scope_type m_scope_type = scope_type::undefined_scope;
		std::string_view scope_name;

	};
}