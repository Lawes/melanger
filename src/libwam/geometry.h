#ifndef GEOMETRY_HEADER
#define GEOMETRY_HEADER

template<class V>
int SIAM( const V& p0, const V& p1, const V& p2) {
	float dx1, dx2, dy1, dy2;

	dx1=p1.x-p0.x; dy1=p1.y-p0.y;
	dx2=p2.x-p0.x; dy2=p2.y-p0.y;

	if( dx1*dy2 > dy1*dx2 ) return 1;
	if( dx1*dy2 < dy1*dx2 ) return -1;
	if( (dx1*dx2 < 0.0) || (dy1*dy2 < 0.0) ) return -1;
	if( (dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2) ) return 1;

	return 0;
}

template<class V>
bool ptInShape(const V&p, const V& p1, const V& p2, const V& p3, const V&p4) {
	return SIAM(p1, p2, p) >=0 
		&& SIAM(p2, p3, p) >= 0 
		&& SIAM(p3, p4, p) >= 0 
		&& SIAM(p4, p1, p) >= 0;	
}

#endif