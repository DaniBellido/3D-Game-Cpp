
#include "Vector3.h"



// vector normalisation
void normalizeVector3f(vector3f* v)
{
	float sum = (float)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);

	if (sum != 0.0f)
	{
		v->x = v->x / sum;
		v->y = v->y / sum;
		v->z = v->z / sum;
	}
}
