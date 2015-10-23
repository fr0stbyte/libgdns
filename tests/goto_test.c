#include<stdio.h>

int main(void) {
	int i = 3;
	if(!i)
		goto mylabel;
	printf("should be skipped\n");
	mylabel:
		printf("in goto label\n");
	printf("last statement\n");
}
