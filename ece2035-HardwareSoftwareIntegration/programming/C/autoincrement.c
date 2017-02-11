/* Auto increment operators. */

#include <stdio.h>
#include <stdlib.h>

int main() {
	int x,y;

	y = 1;
	x = 2;
	y += x++;
	printf("x = %d, y = %d\n", x, y);
	
	y = 1;
	x = 2;
	y = y + (x++);
	printf("x = %d, y = %d\n", x, y);

	y = 1;
	x = 2;
	y += ++x;
	printf("x = %d, y = %d\n", x, y);
	
	y = 1;
	x = 2;
	y = y + (++x);
	printf("x = %d, y = %d\n", x, y);

	return 0;
}
