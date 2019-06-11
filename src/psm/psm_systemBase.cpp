
#include <algorithm>
#include "psm_systemBase.h"

using namespace std;
using namespace PSM;

ParticuleSystemBase::ParticuleSystemBase(int n) :
	actifQ_(false),
	pool_(n),
	firstInactif_(0)
{

	for(int i=0; i<n; ++i)
		pool_[i].id = i;

}


void ParticuleSystemBase::remove(iterator it) {
	if( firstInactif_ > 1 ) {
		swap(*it, pool_[firstInactif_-1]);
	}
	firstInactif_--;

}

int ParticuleSystemBase::getFreePart(float num) const {
	int intPart = static_cast<int>(num);
	int n = static_cast<int>(pool_.size());
	if( firstInactif_ + intPart > n )
		intPart = n - firstInactif_ - 1;

	return intPart;

}

