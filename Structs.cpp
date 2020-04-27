#include "Structs.h"

namespace structs
{
	void structs::loadPolys()
	{
		Poly* dummy = new Poly("name");

		structs::polyStreet.push_back(dummy);
		new (&dummy) Poly;
	}
}
