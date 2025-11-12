#ifndef QUATERNION_H
#define QUATERNION_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "vertex.h"

#define QUATERNION_EPS (1e-4)

typedef struct quaternion
{
	f32 w;
	vertex v;
} quaternion;

// Function to set a quaternion to a given value:
void quaternion_set(quaternion* q_o, f32 w, f32 x, f32 y, f32 z);

// Function that sets a provided quaternion to an identity quaternion;
void quaternion_set_identity(quaternion* q_o);

// Function that copies the contents of a provided quaternion to another provided quaternion:
void quaternion_copy(quaternion* q, quaternion* q_o);

// Function that returns if two quaternions are equal, within a givin margin:
bool quaternion_equal(quaternion* q1, quaternion* q2);

// Function that prints the relevant data from a given quaternion to the console:
void quaternion_printf(quaternion* q);

// Function to calculate the conjugate of the provided quaternion, output is stored in q_o:
void quaternion_conjugate(quaternion* q, quaternion* q_o);

// Function that returns the magnitude of the provided quaternion:
f32 quaternion_magnitude(quaternion* q);

// Function to normalize a quaternion, output is stored in q_o:
void quaternion_normalize(quaternion* q, quaternion* q_o);

// Function to multiply two quaternions, output is stored in q_o:
void quaternion_multiply(quaternion* q1, quaternion* q2, quaternion* q_o);

// Used to rotate a vertex using the provided quaternion:
void quaternion_rotate(quaternion* q, vertex* v, vertex* v_o);

// Used to calculate a smooth transition from q1 to q2 in the form of q_o, over the timeframe t:
void quaternion_slerp(quaternion* q1, quaternion* q2, quaternion* q_o, f32 t);

#endif