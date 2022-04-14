#include "Header.h"
#include "Callable.h"
#include <fstream>
#include <cctype>
#include <cassert>
#include <ranges>

namespace OVD
{
	Header::Header(const std::string& filename) : filename(filename), ppparse_file(filename)
	{
		parse(ppparse_file, ppparse_scopes);
	}

	void Header::parse(const ppparse::source_file& source_file, std::vector<ppparse::graph_node const*>& scopes)
	{
		source_file.print();
		//todo
		std::vector<ppparse::graph_node const*> attributes = source_file.get_elements_of_type(ppparse::node_type::attribute);
		for (ppparse::graph_node const* attribute : attributes)
		{
			ppparse::graph_node* parent_expression = attribute->parent;
			std::string parent_expression_name = std::string(parent_expression->get_name());
			std::string parent_expression_scope = parent_expression->get_qualified_scope();

			std::vector<std::span<ppparse::graph_node * const>> attribute_members = attribute->children_as_parameter_list_members();
			for (std::span<ppparse::graph_node * const> &attribute_member : attribute_members)
			{
				if ((*attribute_member.begin())->get_node_body() == "ovd")
				{
					//it's an ovd attribute
					std::string_view attribute_type = attribute_member[2]->get_node_body();
					//printf("%s", attribute_type.c_str());
					if (attribute_type == "callable")
						callables.push_back(Callable{ .name = parent_expression_name });
					else if (attribute_type == "defined")
						definitions.push_back(Definition(Callable{ .name = parent_expression_name }));
					else if (attribute_type == "variable")
						variables.push_back(Variable());

				}
			}
			std::string scope = attribute->get_qualified_scope();
			printf("%s", scope.c_str());
		}
	}
}