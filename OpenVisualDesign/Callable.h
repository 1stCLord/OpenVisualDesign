#pragma once
#include <vector>
#include <string>
#include "Variable.h"

namespace OVD
{
	struct Callable
	{
		std::string classname;//todo
		std::string name;
		std::vector<Variable> parameters;
		std::string return_type;
		bool is_static;//todo
	};
}