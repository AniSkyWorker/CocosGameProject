#pragma once
#include <vector>

struct SCombinationsBlockData
{
	SCombinationsBlockData();

	std::vector<int> blockKits;
	std::vector<int> blockWidths;
	std::vector<int> blockHeights;
	std::vector<int> blockTypes;
};

