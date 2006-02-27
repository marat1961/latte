#ifndef INTEGRALHULL__H
#define INTEGRALHULL__H

#include <stdarg.h>
#include "myheader.h"
#include "ramon.h"
#include <list>

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <algorithm>
#include <time.h>

listCone* FindRationalFunction(listCone* cones, vec_ZZ a, vec_ZZ cost, int numOfVars);
listVector* Push_Vector(listVector* head, listVector* tail, int numOfVars);
vec_ZZ SolveIP(listCone* cones, listVector* matrix,  vec_ZZ cost, int numOfVars, int SINGLE_CONE);
int CheckVertices(listVector* vertices, listVector* newVertices);
listVector* GetVertices(listCone* cones,  listVector* matrix,  listVector* hyperplane, int numOfVars, int flag);
listVector* GetHRepresentation(listVector* vertices, int numOfVars);
listVector* IntegralHull(listCone* cones, listVector* matrix, int numOfVars);
ZZ Calculate_Polytope_Width (listCone *cones,listVector *matrix,int numOfVars);
#endif


