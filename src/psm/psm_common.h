#ifndef PSM_COMMON_HEADER
#define PSM_COMMON_HEADER

#include <algorithm>
#include "trigocossin.h"

namespace PSM {

template <class T>
T abs(T val) {
	return std::max(-val, val);
}


template< class T>
T clamp( T val, T low, T hight){

	return std::max(low, std::min(val, hight));
}

extern TrigoCosSin TableCS;

}

#endif
