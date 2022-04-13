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
			//todo get lists of the ovd attributes
			std::string scope = attribute->get_qualified_scope();
			printf("%s", scope.c_str());
		}
	}
}