#ifndef RANDOM_HEADER
#define RANDOM_HEADER

#include <cstddef>


class Random
{
public:
	Random();
	static void Seed(unsigned int seed);
	//! not inclusive
	static int Rand();
	static int Int(int start, int end);
	static float Float(float start, float end);
	static float Percent();

	template< class IteratorModel>
	static IteratorModel randIterator(IteratorModel it, std::size_t n) {
		return it+Rand()%n;
	}

private:
	//static Random *instance;
};

#endif
