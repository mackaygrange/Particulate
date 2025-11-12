#include "quaternion.h"


void quaternion_set(quaternion* q_o, f32 w, f32 x, f32 y, f32 z)
{
	ASSERT(q_o != NULL, "Output cannot be NULL.");
	q_o->w = w;
	q_o->v.x = x;
	q_o->v.y = y;
	q_o->v.z = z;
}

void quaternion_set_identity(quaternion* q_o)
{
	quaternion_set(q_o, 1, 0, 0, 0);
}

void quaternion_copy(quaternion* q, quaternion* q_o)
{
	quaternion_set(q_o, q->w, q->v.x, q->v.y, q->v.z);
}

bool quaternion_equal(quaternion* q1, quaternion* q2)
{
	bool w_e = fabs(q1->w - q2->w)     <= QUATERNION_EPS;
	bool x_e = fabs(q1->v.x - q2->v.x) <= QUATERNION_EPS;
	bool y_e = fabs(q1->v.y - q2->v.y) <= QUATERNION_EPS;
	bool z_e = fabs(q1->v.z - q2->v.z) <= QUATERNION_EPS;

	return w_e && x_e && y_e && z_e;
}

void quaternion_printf(quaternion* q)
{
	printf("w: %.3f, x: %.3f, y: %.3f, z: %.3f", q->w, q->v.x, q->v.y, q->v.z);
}

f32 quaternion_magnitude(quaternion* q)
{
	ASSERT(q != NULL, "Output cannot be NULL.");
	return (f32)sqrt((q->w * q->w) + (q->v.x * q->v.x) + (q->v.y * q->v.y) + (q->v.z * q->v.z));
}

void quaternion_conjugate(quaternion* q, quaternion* q_o)
{
	ASSERT(q != NULL, "Output cannot be NULL.");
	q_o->w   =  q->w  ; 
	q_o->v.x = -q->v.x;
	q_o->v.y = -q->v.y;
	q_o->v.z = -q->v.z;
}

void quaternion_normalize(quaternion* q, quaternion* q_o)
{
	ASSERT(q_o != NULL, "Output cannot be NULL.");

	f32 m = quaternion_magnitude(q);
	quaternion_set(q_o, q->w / m, q->v.x / m, q->v.y / m, q->v.z / m);
}

void quaternion_multiply(quaternion* q1, quaternion* q2, quaternion* q_o) 
{
	ASSERT(q_o != NULL, "Output cannot be NULL.");

	quaternion q_t;

	q_t.w =   (q1->w   * q2->w  ) - (q1->v.x * q2->v.x) - (q1->v.y * q2->v.y) - (q1->v.z * q2->v.z);
	q_t.v.x = (q1->v.x * q2->w  ) + (q1->w   * q2->v.x) + (q1->v.y * q2->v.z) - (q1->v.z * q2->v.y);
	q_t.v.y = (q1->w   * q2->v.y) - (q1->v.x * q2->v.z) + (q1->v.y * q2->w  ) + (q1->v.z * q2->v.x);
	q_t.v.z = (q1->w   * q2->v.z) + (q1->v.x * q2->v.y) - (q1->v.y * q2->v.x) + (q1->v.z * q2->w  );

	*q_o = q_t;
}

void quaternion_rotate(quaternion* q, vertex* v, vertex* v_o)
{
	ASSERT(v_o != NULL, "Output cannot be NULL.");

	vertex v_t;

	f32 ww = q->w   * q->w  ;
	f32 xx = q->v.x * q->v.x;
	f32 yy = q->v.y * q->v.y;
	f32 zz = q->v.z * q->v.z;
	f32 wx = q->w   * q->v.x;
	f32 wy = q->w   * q->v.y;
	f32 wz = q->w   * q->v.z;
	f32 xy = q->v.x * q->v.y;
	f32 xz = q->v.x * q->v.z;
	f32 yz = q->v.y * q->v.z;

	v_t.x = (       ww * v->x) + (2.0f * wy * v->z) - (2.0f * wz * v->y) + (       xx * v->x) + (2.0f * xy * v->y) + (2.0f * xz * v->z) - (       zz * v->x) - (yy * v->x);
	v_t.y = (2.0f * xy * v->x) + (       yy * v->y) + (2.0f * yz * v->z) + (2.0f * wz * v->x) - (       zz * v->y) + (       ww * v->y) - (2.0f * wx * v->z) - (xx * v->y);
	v_t.z = (2.0f * xz * v->x) + (2.0f * yz * v->y) + (       zz * v->z) - (2.0f * wy * v->x) - (       yy * v->z) + (2.0f * wx * v->y) - (       xx * v->z) + (ww * v->z);

	*v_o = v_t;
}

void quaternion_slerp(quaternion* q1, quaternion* q2, quaternion* q_o, f32 t)
{
	quaternion q_t;

	f32 cos_half_theta = (q1->w * q2->w) + (q1->v.x * q2->v.x) + (q1->v.y * q2->v.y) + (q1->v.z * q2->v.z);

	if (fabs(cos_half_theta) >= 1.0f)
	{
		quaternion_copy(q1, q_o);
		return;
	}

	f32 half_theta = (f32)acos(cos_half_theta);
	f32 sin_half_theta = (f32)sqrt(1.0f - cos_half_theta * cos_half_theta);

	if (fabs(sin_half_theta) < QUATERNION_EPS)
	{
		q_t.w   = (q1->w   * 0.5f) + (q2->w   * 0.5f);
		q_t.v.x = (q1->v.x * 0.5f) + (q2->v.x * 0.5f);
		q_t.v.y = (q1->v.y * 0.5f) + (q2->v.y * 0.5f);
		q_t.v.z = (q1->v.z * 0.5f) + (q2->v.z * 0.5f);
	}
	else
	{
		f32 r_a = (f32)(sin((1 - t) * half_theta) / sin_half_theta);
		f32 r_b = (f32)(sin((    t) * half_theta) / sin_half_theta);

		q_t.w   = (q1->w   * r_a) + (q2->w   * r_b);
		q_t.v.x = (q1->v.x * r_a) + (q2->v.x * r_b);
		q_t.v.y = (q1->v.y * r_a) + (q2->v.y * r_b);
		q_t.v.z = (q1->v.z * r_a) + (q2->v.z * r_b);
	}
	*q_o = q_t;
}
