/*
   Phobia Motor Controller for RC and robotics.
   Copyright (C) 2015 Roman Belov <romblv@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "m.h"

void rotatef(float y[2], float angle, const float x[2])
{
	float		q, s, c, a, b;

	q = angle * angle;
	s = angle - q * angle * (.16666667f - 8.3333333e-3f * q);
	c = 1.f - q * (.5f - 4.1666667e-2f * q);

	a = c * x[0] - s * x[1];
	b = s * x[0] + c * x[1];

	q = (3.f - a * a - b * b) * .5f;
	y[0] = a * q;
	y[1] = b * q;
}

float arctanf(float y, float x)
{
	const float	p[] = {.25065566f, .93348042f, 1.5707963f};
	float		y_abs, f = 0.f;

	y_abs = fabsf(y);

	if (x < 0.f) {

		f = x;
		x = y_abs;
		y_abs = - f;
		f = p[2];
	}

	if (y_abs < x)

		f += (p[0] * y_abs + p[1]) * y_abs;
	else
		f += p[2] - (p[0] * x + p[1]) * x;

	return (y < 0.f) ? - f : f;
}

static float	kpoly[8] = {

	-1.37729499e-4f,
	-2.04509846e-4f,
	8.63928854e-3f,
	-2.43287243e-4f,
	-1.66562291e-1f,
	-2.23787462e-5f,
	1.00000193e+0f,
	-3.55250780e-8f,
};

float ksinf(float angle)
{
        float           u;
        int             m = 0;

        if (angle < 0.f) {

                m = 1;
                angle = - angle;
        }

        if (angle > (MPIF / 2.f))
                angle = MPIF - angle;

        u = kpoly[1] + kpoly[0] * angle;
        u = kpoly[2] + u * angle;
        u = kpoly[3] + u * angle;
	u = kpoly[4] + u * angle;
	u = kpoly[5] + u * angle;
	u = kpoly[6] + u * angle;
	u = kpoly[7] + u * angle;
	
	return m ? - u : u;
}

float kcosf(float angle)
{
        float           u;
        int             m = 0;

        if (angle < 0.f)
                angle = - angle;

        if (angle > (MPIF / 2.f)) {

                m = 1;
                angle = MPIF - angle;
        }

	angle = (MPIF / 2.f) - angle;

	u = kpoly[1] + kpoly[0] * angle;
	u = kpoly[2] + u * angle;
	u = kpoly[3] + u * angle;
	u = kpoly[4] + u * angle;
	u = kpoly[5] + u * angle;
	u = kpoly[6] + u * angle;
	u = kpoly[7] + u * angle;

	return m ? - u : u;
}
