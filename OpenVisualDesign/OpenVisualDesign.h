// OpenVisualDesign.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vector>
#include <memory>
#include "Header.h"

class OpenVisualDesign
{
public:
	OpenVisualDesign();

	std::vector<std::unique_ptr<OVD::Header>> headers;
};
