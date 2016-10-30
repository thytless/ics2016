#include "trap.h"
#include "FLOAT.h"

FLOAT f(FLOAT x) { 
	/* f(x) = 1/(1+25x^2) */
	return F_div_F(int2F(1), int2F(1) + F_mul_int(F_mul_F(x, x), 25));
}

FLOAT computeT(int n, FLOAT a, FLOAT b, FLOAT (*fun)(FLOAT)) {
	int k;
	FLOAT s,h;
	h = F_div_int((b - a), n);//h = 0.2
	s = F_div_int(fun(a) + fun(b), 2 );
//	nemu_assert(Fabs( s - f2F(0.038461)  ) < f2F(1e-4));

//	nemu_assert(1/16 == 0.0625);

	for(k = 1; k < n; k ++) {
		s += fun(a + F_mul_int(h, k));
		if(k == 1)
			nemu_assert(Fabs(fun(f2F(-0.8)) - f2F(0.0588235)) < f2F(1e-4));
	}
	s = F_mul_F(s, h);
	return s;
}

int main() { 
	FLOAT a = computeT(10, f2F(-1.0), f2F(1.0), f);
	FLOAT ans = f2F(0.551222);

	nemu_assert(Fabs(a - ans) < f2F(1e-4));

	return 0;
}
