#include <assert.h>

struct vector {
	int length;
	double * content; 
};

typedef struct vector vector;

vector * vector_create(int length, double * content) {
	vector *a = (vector *)malloc(sizeof(vector));
	a->content = (double *)malloc(sizeof(double) * length);
	a->length = length;
	int i = 0;
	for(i = 0; i < length; i++) {
		a->content[i] = content[i];
	} 
	return a;
}

vector * vector_create_space(vector * a, int length) {
	assert(a == NULL);
	a = (vector *)malloc(sizeof(vector));
	a -> length = length;
	a -> content = (double *)malloc(sizeof(double) * length);
	return a;
}

vector * vector_content_copy(vector * a, double * content, int length) {
	assert(length <= a->length);
	int i = 0;
	for(i = 0; i < length; i++) {
		a->content[i] = content[i];
	} 
	return a; 
}

vector * vector_addition (vector * a, vector * b, vector *c) {
	assert(a->length == b->length);
	assert(c->length == b->length);
	int i = 0;
	for(i = 0; i < a->length; i++ ) {
		c->content[i] = a->content[i] + b->content[i];
	}
	return c;
}

vector * vector_subtraction (vector * a, vector * b, vector * c) {
	assert(a->length == b->length);
	assert(c->length == a->length);
	int i = 0;
	for(i = 0; i < a->length; i++ ) {
		c->content[i] = (a->content[i]) - (b->content[i]);
	}
	return c;
}

double vector_inner(vector * a, vector * b) {
	assert(a->length == b->length);
	double rv = 0;
	int i = 0;
	for(i = 0; i < a->length; i++) {
		rv += a->content[i] * b->content[i];
	}
	return rv;
}

vector * vector_scalar_mult(vector * a, vector * b, double constant) {
	int i = 0;
	for(i = 0; i < a->length; i++) {
		b->content[i] = constant * a->content[i];
	}
	return b;
}

void print_vector(vector * a) {
	int i = 0;
	for(i = 0; i < a->length; i++) {
 		if( i == a->length - 1) 
			printf("%lf\n", a->content[i]);
		else
			printf("%lf,", a->content[i]);
	}
}

vector * vector_copy (vector * a) {
	vector * c = (vector *)malloc(sizeof(vector));
	c->length = a->length;
	c->content = (double *)malloc(sizeof(double) * a->length);
	int i = 0;
	for(i = 0; i < a->length; i++ ) {
		c->content[i] = a->content[i];
	}
	return c;
}

void vector_free(vector *a) {
	free(a->content);
	free(a);
}
