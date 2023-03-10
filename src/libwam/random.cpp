#include "random.h"
#include <time.h>
#include <stdlib.h>


Random::Random()
{
	srand(static_cast<unsigned int>(time(NULL)));
}

void Random::Seed(unsigned int seed)
{
	srand(seed);
}

int Random::Rand() {
	return rand();
}

int Random::Int(int start, int end)
{
	return start + (rand() % (end-start));
}

float Random::Float(float start, float end)
{
	return ((((float) rand()) / (float) RAND_MAX) * (end-start)) + start;
}

float Random::Percent()
{
	return ((float) rand()) / (float) RAND_MAX;
}