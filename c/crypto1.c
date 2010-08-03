/**
 * input: t
 * p = 4*10^9 + 7 is prime
 * t^2 =modp= y
 *
 * little fermat: x^(p-1) =modp= 1
 * x^((p-1)/2) =modp= +-1
 * x^((p+1)/2) =modp= +-x
 * (x^((p+1)/4))^2 =modp= +-x
 * -> t^2 =modp= +-y with t = y^((p+1)/4)
 *
 * https://www.spoj.pl/problems/CRYPTO1/
 * Schier Michael
 */

#include <stdio.h>
#include <time.h>


int main(int argc, char** args)
{
	unsigned long long int input;
	scanf("%llu\n", &input);
	
	unsigned long long int p = 4000000007ll;
	unsigned long long int q = 1000000002ll;
	unsigned long long int r = 1;
	unsigned long long int y = input;
	// calculate y^((p+1)/4) mod p = y^q mod p
	// do a fast exponentiation
	while(q != 0)
	{
		if(q%2 > 0)
			r = (r * y) % p;
		y = (y * y) % p;
		q = q / 2;
	}
	
	if(2*r > p) // take the smaller root of y
		r = p - r;
	
	const time_t t = (time_t)r;
	printf("%s", asctime(gmtime(&t)));
	
	return 0;
}

