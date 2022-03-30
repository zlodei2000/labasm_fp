#include <iostream> 
using namespace std;
extern  "C" int power2(int num, int power);

int main()
{
	// 
	// change 2
	double a, b, c;
	const char* msg1 = "x = %f\t f(x) = %f\n";
	const char* msg2 = "x = %f\t f(x) = n/a\n";
	const double dzero = 0.0;
	const double done = 1.0;
	const double dfour = 4.0;
	const double dfive = 5.0;
	const double dten = 10.0;
	cout << "power 2:" << power2(1, 1) << endl;

	cout << "sizeof char " << sizeof(char) << endl;
	cout << "Enter a: ";
	cin >> a;
	cout << "Enter b: ";
	cin >> b;

	do {
		cout << "Enter c( c != 4.0): ";
		cin >> c;
	} while (c == 4.0);

	double d, e, i;
	do {
		cout << "Enter d: ";
		cin >> d;
		cout << "Enter e (e > d): ";
		cin >> e;
	} while (d >= e);

	do {
		cout << "Enter i ( (d + i) < e ): ";
		cin >> i;
	} while (d + i >= e);

	double f;
	double x;

	printf("\n cpp output:\n");

	for (x = d; x <= e; x += i) {
		if (x + dfive < dzero && c == dzero) {
			if (x != dzero) {
				f = done / a * x - b;
				printf(msg1, x, f);
			}
			else {
				printf(msg2, x);
			}
		}
		else if (x + dfive > 0 && c != dzero) {
			if (x != dzero) {
				f = (x - a) / x;
				printf(msg1, x, f);
			}
			else {
				printf(msg2, x);
			}
		}
		else {
			f = dten * x / (c - dfour);
			printf(msg1, x, f);
		}
	}

	printf("\n Assembler output:\n");

	__asm {
		//	for (double x = d; x <= e; x += i) {

			movsd	xmm0, d
			movsd	x, xmm0
			jmp	SHORT $LN13main
$LN11main:
			movsd	xmm0, x
			addsd	xmm0, i
			movsd	x, xmm0
$LN13main:
			movsd	xmm0, e
			comisd	xmm0, x
			jb	$LN12main

		// if (x + dfive < dzero && c == dzero) {

			movsd	xmm0, x
			addsd	xmm0,  dfive
			xorps	xmm1, xmm1
			comisd	xmm1, xmm0
			jbe	$LN14main
			movsd	xmm0,  c
			ucomisd	xmm0,  dzero
			lahf
			test	ah, 68 // 00000044H
			jp	$LN14main
		// if (x != dzero) {
			movsd	xmm0,  x
			ucomisd	xmm0,  dzero
			lahf
			test	ah, 68 // 00000044H
			jnp	SHORT $LN16main
		// f = done / a * x - b;
			movsd	xmm0,  done
			divsd	xmm0,  a
			mulsd	xmm0,  x
			subsd	xmm0,  b
			movsd	 f, xmm0
		// printf(msg1, x, f);

			sub	esp, 8
			movsd	xmm0,  f
			movsd	[esp], xmm0
			sub	esp, 8
			movsd	xmm0,  x
			movsd	[esp], xmm0
			mov	eax, DWORD PTR msg1
			push	eax
			call	printf
			add	esp, 20 
			jmp	SHORT $LN17main
$LN16main:
		// else {
		// printf(msg2, x);

			sub	esp, 8
			movsd	xmm0,  x
			movsd	[esp], xmm0
			mov	eax, DWORD PTR msg2
			push	eax
			call	printf
			add	esp, 12 // 0000000cH
$LN17main:

			jmp	$LN19main

$LN14main:

		// else if (x + dfive > 0 && c != dzero) {

			movsd	xmm0,  x
			addsd	xmm0,  dfive
			comisd	xmm0,  dzero
			jbe	$LN18main
			movsd	xmm0,  c
			ucomisd	xmm0,  dzero
			lahf
			test	ah, 68; 00000044H
			jnp	SHORT $LN18main

		//	if (x != dzero) {
			movsd	xmm0,  x
			ucomisd	xmm0,  dzero
			lahf
			test	ah, 68 // 00000044H
			jnp	SHORT $LN20main
		// f = (x - a) / x;

			movsd	xmm0,  x
			subsd	xmm0,  a
			divsd	xmm0,  x
			movsd	 f, xmm0

		//	printf(msg1, x, f);

			sub	esp, 8
			movsd	xmm0,  f
			movsd	[esp], xmm0
			sub	esp, 8
			movsd	xmm0,  x
			movsd	[esp], xmm0
			mov	eax, DWORD PTR msg1
			push	eax
			call	printf
			add	esp, 20

			jmp	SHORT $LN21main
$LN20main:
		//	else {
		//	printf(msg2, x);
			sub	esp, 8
			movsd	xmm0,  x
			movsd	[esp], xmm0
			mov	eax, DWORD PTR msg2
			push	eax
			call	printf
			add	esp, 12

$LN21main:

			jmp	SHORT $LN19main
$LN18main:
		//	else {
		//	f = dten * x / (c - dfour);

			movsd	xmm0,  dten
			mulsd	xmm0,  x
			movsd	xmm1,  c
			subsd	xmm1,  dfour
			divsd	xmm0, xmm1
			movsd	 f, xmm0

		// printf(msg1, x, f);

			sub	esp, 8
			movsd	xmm0,  f
			movsd	[esp], xmm0
			sub	esp, 8
			movsd	xmm0,  x
			movsd	[esp], xmm0
			mov	eax, DWORD PTR msg1
			push	eax
			call	printf
			add	esp, 20
$LN19main:
		jmp	$LN11main

$LN12main:

	}
}
