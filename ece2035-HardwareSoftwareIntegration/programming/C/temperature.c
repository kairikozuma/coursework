/* Data types: Temperature conversion. */

#include <stdio.h>
int main()
{
	int F;
	double C;
	for(F=0; F<250; F+=10)
	{
		C = (F-32)*(5.0/9);
		printf("F = %3d, C = %4.2f\n", F, C);
	} 
	return 0;
}
