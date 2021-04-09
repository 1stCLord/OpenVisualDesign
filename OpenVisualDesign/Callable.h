#pragma once
#include <vector>
#include <string>
#include "Variable.h"

namespace OVD
{
	struct Callable
	{
		std::string name;
		std::vector<Variable> parameters;
		std::string return_type;
	};
}