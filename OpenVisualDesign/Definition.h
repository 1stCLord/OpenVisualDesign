#pragma once
#include <vector>
#include <string>
#include "Callable.h"

namespace OVD
{
	class Definition
	{
	public:
		class Callee
		{
		public:
			Callable const * callable;
			std::vector<uint8_t> parameter_sources;
		};

		Definition(const Callable& callable) : callable(callable) {}

		const Callable& get_callable() const { return callable; }
		std::vector<Callee>& get_callees() { return callees; }
	private:
		Callable callable;
		std::vector<Callee> callees;
	};
}