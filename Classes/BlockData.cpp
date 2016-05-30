#include "BlockData.h"

SCombinationsBlockData::SCombinationsBlockData()
{
	blockKits = { 1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3 };
	blockWidths = { 2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4 };
	blockHeights = { 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,4 };
	blockTypes = { 1,2,3,4,1,3,2,4,3,2,1,4,2,3,1,4,2,3,1,2,3,2,3,4,1,2,4,3,1,3,1,4,2,4,2,1,2,3 };
}
