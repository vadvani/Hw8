#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct image{
	int width;
	int height;
	int** image;
};

struct image* imageCreate(void) {
	struct image* i;
	int c;

	i = malloc(sizeof(struct image));
	assert(i);

	scanf ("P5 %d %d 255\n", &(i->width), &(i->height));
	
	i->image = malloc(sizeof(int*) * i->height); /*using right sizeof here???*/
	assert(i->image);
	for (int j = 0; j < i->width; j++) {
		i->image[j] = malloc(sizeof(int) * i->width);
		assert(i->image[j]);
	}

	for (int j=0; j < i->height; j++) {
		for (int k=0; k < i->width; k++) {
			c = getchar();
			i->image[j][k] = c;
		}
	}

	return i;
}

void printImage(struct image* i) {
	printf("P5 %d %d 255\n", i->width, i->height);
	for (int j=0; j < i->height; j++) {
		for (int k=0; k < i->width; k++) {
			printf("%d", i->image[j][k]);
		}
	}
}

int main (int argc, char** argv) {

	struct image* i;
	
	i = imageCreate();
	printImage(i);

	return 0;
}