#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include "Variable.h"
#include "Callable.h"
#include "Definition.h"

namespace OVD
{
	enum Attribute : int
	{
		none = 0,
		variable = 1,
		callable = 2,
		definition = 4
	};

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

		std::vector<Variable> variables;
		std::vector<Callable> callables;
		std::vector<Definition> definitions;

		static void parse(const std::string& header_string, std::vector<Variable>& variables, std::vector<Callable>& callables, std::vector<Definition>& definitions);
		static Attribute parse_attributes(const std::string_view& attribute_string);
		static Attribute parse_attribute_component(const char* begin, const char* end);

		static void check_attribute_errors(Attribute attributes, bool hasParameters, bool hasFunctionBody);

		static void parse_type_and_name(const std::string_view& expression, std::string_view& type, std::string_view& name);
		static std::vector<Variable> parse_parameters(const std::string_view& parameter_list);

		static std::string_view trim_whitespace(const std::string_view& to_trim);
	};
}