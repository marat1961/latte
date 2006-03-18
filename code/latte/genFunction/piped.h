#ifndef GENFUNCTION_PIPED_H
#define GENFUNCTION_PIPED_H

vec_ZZ movePoint(vec_ZZ, rationalVector*, rationalVector*, vec_ZZ*, int, int);
listVector* pointsInParallelepiped(listCone *cones, int numOfVars);
listVector* pointsInParallelepipedOfUnimodularCone(rationalVector*, 
						   listVector*, int);

/* Compute the latticePoints slot of CONE. */
void computePointsInParallelepiped(listCone *cone, int numOfVars);

/* For all cones in the linked list CONES, compute their latticePoints
   slot. */
void computePointsInParallelepipeds(listCone *cones, int numOfVars);

#endif
