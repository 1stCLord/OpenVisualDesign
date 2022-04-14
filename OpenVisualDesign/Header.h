#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include "Variable.h"
#include "Callable.h"
#include "Definition.h"
#include "source_file.h"

namespace OVD
{
	class Header
	{
	public:
		Header(const std::string& filename);

		const std::vector<Variable>& get_variables() const { return variables; }
		const std::vector<Callable>& get_callables() const { return callables; }
		const std::vector<Definition>& get_definitions() const { return definitions; }
		std::vector<Definition>& get_definitions() { return definitions; }

		const std::filesystem::path& get_filename() const { return filename; }

	private:
		std::filesystem::path filename;
		std::vector<Callable> callables;
		std::vector<Definition> definitions;
		std::vector<Variable> variables;

		ppparse::source_file ppparse_file;
		std::vector<ppparse::graph_node const *> ppparse_scopes;
		void parse(const ppparse::source_file& source_file, std::vector<ppparse::graph_node const *>& scopes);
	};
}