#include "FLOAT.h"
FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int sign_a = (a < 0);
	int sign_b = (b < 0);
	FLOAT sign_r = sign_a ^ sign_b;
	FLOAT abs_a = sign_a ? -a : a;
	FLOAT abs_b = sign_b ? -b : b;
	FLOAT abs = (abs_a >> 0x8) * (abs_b >> 0x8);
	return sign_r ? -abs : abs;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */
	int _edx = a >> 0x10; 
	int _eax = a << 0x10;
	int eax_, edx_;
	asm volatile ("idiv %2" : "=a"(eax_), "=d"(edx_) : "r"(b), "a"(_eax), "d"(_edx));
	
	return eax_;
}

FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */
	Float a_;
	a_._float = a;
	unsigned int _a = a_._int;
 	int s = (_a >> 0x1f) & 0x1;
	signed char exp =((_a >> 0x17) & 0xff) - 0x7f;
 	int sig = _a & 0x7fffff;
 	if(exp != -0x7f)
 		sig |= (0x1 << 0x17);
	int shift = exp - 7;
	FLOAT ret;
	if(shift > 0)
		ret = sig << shift;
	else if(shift < 0)
		ret = sig >> (-shift);
	else
		ret = sig;
	return s ? -ret : ret;
}

FLOAT Fabs(FLOAT a) {
	int s = (a >> 0x1f) & 0x1;
	return s ? -a : a;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

