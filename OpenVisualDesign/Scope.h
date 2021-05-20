#pragma once
#include <vector>
#include <string>
#include <string_view>
#include "Attribute.h"
#include "Variable.h"
#include "Callable.h"
#include "Definition.h"

namespace OVD
{
	class Scope
	{
	public:
		enum scope_type
		{
			namespace_scope,
			class_scope,
			unknown_scope
		};

		const std::vector<Variable>& get_variables() const { return variables; }
		const std::vector<Callable>& get_callables() const { return callables; }
		const std::vector<Definition>& get_definitions() const { return definitions; }
		std::vector<Definition>& get_definitions() { return definitions; }

		Scope(const std::string_view& scope_string);

		static scope_type get_scope_type(const std::string_view& substring, std::string &scope_name);
	private:
		std::string name;

		scope_type scope_type;

		std::vector<Variable> variables;
		std::vector<Callable> callables;
		std::vector<Definition> definitions;

		static void parse(const std::string_view& scope_string, std::vector<Variable>& variables, std::vector<Callable>& callables, std::vector<Definition>& definitions);
		static Attribute parse_attributes(const std::string_view& attribute_string);
		static Attribute parse_attribute_component(const char* begin, const char* end);

		static void check_attribute_errors(Attribute attributes, bool hasParameters, bool hasFunctionBody);

		static void parse_type_and_name(const std::string_view& expression, std::string_view& type, std::string_view& name);
		static std::vector<Variable> parse_parameters(const std::string_view& parameter_list);

		static std::string_view trim_whitespace(const std::string_view& to_trim);
	};
}