#include "dtl_Utility.h"
#include <ctime>

#ifdef _DEBUG
	const unsigned int SOME_RAND_SEED = 180692475;
#else
	const unsigned int SOME_RAND_SEED = static_cast<unsigned int>(time(NULL));
#endif

class CRandInit
{
public:
	CRandInit()
	{
		srand(SOME_RAND_SEED);
	}
};

CRandInit RandInit;