// http://en.wikipedia.org/wiki/Cubic_Hermite_spline

#ifndef INTERPSPLINE_HEADER
#define INTERPSPLINE_HEADER


#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>


namespace wam {

typedef std::vector<float> VectF;

class nothing {};

struct DiffFiniteDifference {
	void operator() (float dt1, float dt2, float &a, float& b, float& c) const {
		c = -0.5*dt1/dt2;
		a = 0.5;
		b = -c - .5;

	}

};

struct DiffCardinal {

	void operator() (float dt1, float dt2, float &a, float& b, float& c) const {
		a = (1.0-m_c)*dt1/(dt1+dt2);
		b = 0.0;
		c = -a;
	}

	DiffCardinal(float c) : m_c(c) {}
	private:
		float m_c;
};

struct DiffCatmullRom {

	void operator() (float dt1, float dt2, float &a, float& b, float& c) const {
		a = dt1/(dt1+dt2);
		b = 0.0;
		c = -a;

	}

};

template<class T>
struct Constant {
	typedef std::vector<T> VectT;
	static const int Deg = 1;
	void GetCoef(const VectF& t, const VectT& y, VectT& coef, bool loop = false) {
		coef.resize(1 * (t.size()-1));
		copy(y.begin(), y.end()-1, coef.begin());
	}
};

template<class T>
struct Linear {
	typedef std::vector<T> VectT;
	static const int Deg = 2;

	void GetCoef(const VectF& t, const VectT& y, VectT& coef, bool loop = false) {
	    std::size_t n = t.size();
		coef.resize(2 * (n-1));

		if( n < 2 ) return;


		for(std::size_t i = 0; i<(n-1); ++i) {
			coef[2*i] = (y[i+1] - y[i]) / (t[i+1] - t[i]);
			coef[2*i + 1 ] = y[i];
//			std::cout << "compute : " << coef[2*i] << ' ' << coef[2*i + 1 ] << std::endl;
		}
	}

};

template<class T>
void bspline_evalCoef(const T& ym1, const T& y0, const T& y1, const T& y2, float dt, std::vector<T>& pp ) {

	float invDt = 1.f/dt;
	float invDt2 = invDt*invDt;
	float invDt3 = invDt2*invDt;

	pp[0] = 1.f/6.f * invDt3 * ( -ym1 + 3.f * y0 - 3.f * y1 + y2);
	pp[1] = 1.f/2.f * invDt2 * ( ym1 - 2.f * y0 + y1);
	pp[2] = 1.f/2.f * invDt  * ( -ym1            + y1);
	pp[3] = 1.f/6.f * ( ym1 + 4.f * y0 +  y1);

}


template<class T>
struct BSpline {
	static const int Deg = 4;
	typedef std::vector<T> VectT;

	void GetCoef(const VectF& t, const VectT& y, VectT& coef, bool loop = false) {
		size_t n = t.size();

		coef.resize(4 * (n-1));

        if( n < 3 ) return;

		VectT v(4);

		T ptDeb, ptEnd;

		if( loop ) {
            ptDeb = y[n-2];
            ptEnd = y[1];
		}
        else {
            ptDeb = 2*y[0]-y[1];
            ptEnd = 2*y[n-1]-y[n-2];
        }



		bspline_evalCoef(ptDeb, y[0], y[1], y[2], t[1] - t[0], v);
		std::copy(v.begin(), v.end(), coef.begin());

		for(size_t i=1; i<(n-2); ++i) {

			bspline_evalCoef(y[i-1], y[i], y[i+1], y[i+2], t[i+1]-t[i], v);
			std::copy(v.begin(), v.end(), coef.begin()+4*i);
		}

		bspline_evalCoef(y[n-3], y[n-2], y[n-1], ptEnd, t[n-1]-t[n-2], v);
		std::copy(v.begin(), v.end(), coef.begin()+4*(n-2));

	}

};


template<class T, class DifEstimate>
void hermite_evalCoef(const T& ym1, const T& y0, const T& y1, const T& y2,
	   	float dt0, float dt1, float dt2,
	   	std::vector<T>& pp, const DifEstimate& diff ) {

	float a,b,c, aa, bb, cc;

	float invDt = 1./dt1;
	float invDt2 = invDt*invDt;
	float invDt3 = invDt2*invDt;

	diff(dt1, dt0, a, b, c);
	diff(dt2, dt1, aa, bb, cc);

	pp[0] = invDt3 * ( c * ym1 + (2.f+b+cc) * y0 + (-2.f+a+bb)* y1 + aa * y2);
	pp[1] = invDt2 * ( -2.f*c * ym1 + (-3.f-2.f*b-cc) * y0 + (3.f-2.f*a-bb) * y1 - aa * y2);
	pp[2] = invDt  * ( c * ym1 + b *  y0 +  a * y1 );
	pp[3] = y0;

}



template<class T>
struct Hermite {
	typedef std::vector<T> VectT;
	typedef DiffFiniteDifference DifEstimate;
	static const int Deg = 4;

	void GetCoef(const VectF& t, const VectT& y, VectT& coef, bool loop = false) {
		std::size_t n = t.size();
		coef.resize(4 * (n-1));

        if( n < 3 ) return;

		VectT v(4);

		T ptDeb, ptEnd;
		float dtDeb, dtEnd;

		if( loop ) {
            ptDeb = y[n-2];
            ptEnd = y[1];
            dtDeb = t[n-1]-t[n-2];
            dtEnd = t[1] - t[0];
		}
        else {
            ptDeb = 2*y[0]-y[1];
            ptEnd = 2*y[n-1]-y[n-2];
            dtDeb = t[1] - t[0];
            dtEnd = t[n-1]-t[n-2];
        }

		hermite_evalCoef(ptDeb, y[0], y[1], y[2], t[1] - t[0], t[1] - t[0], t[2] - t[1], v, m_diff_op);
		std::copy(v.begin(), v.end(), coef.begin());


		for(size_t i=1; i<(n-2); ++i) {
			hermite_evalCoef(y[i-1], y[i], y[i+1], y[i+2], t[i]-t[i-1], t[i+1]-t[i], t[i+2]-t[i+1], v, m_diff_op);
			std::copy(v.begin(), v.end(), coef.begin()+4*i);
		}

		hermite_evalCoef(y[n-3], y[n-2], y[n-1], ptEnd, t[n-2]-t[n-3], t[n-1]-t[n-2],t[n-1]-t[n-2], v, m_diff_op);
		std::copy(v.begin(), v.end(), coef.begin()+4*(n-2));

	}

	Hermite() {}
	//Hermite(float p) : m_diff_op( DifEstimate(p) ) {}

	private:
		DifEstimate m_diff_op;
};



template<class T, template <class> class InterpImpl >
class Interp1 {

	private:
		typedef std::vector<T> VectT;

		InterpImpl<T> m_implSpline;
		VectF m_nodes;
		VectT m_pt;

		VectT m_coef;
		T valMin_, valMax_;
		bool loop_;


		T eval(float dt, std::size_t d) const {

			typename VectT::const_iterator itT = m_coef.begin() + d* InterpImpl<T>::Deg;
			T res = *itT;
			++itT;
			for(int i=1; i<InterpImpl<T>::Deg; ++i, ++itT)
				res = res * dt + *itT;

			return res;
		}


	public:
		Interp1():loop_(false) {};
		Interp1(float param1) : m_implSpline(param1) { };

		void init(const VectF& tdata, const VectT& ydata) {
			valMin_ = ydata.front();
			valMax_ = ydata.back();

			m_nodes = tdata;
			m_pt = ydata;

		    m_implSpline.GetCoef(m_nodes, m_pt, m_coef, loop_);

		}

		void addPt(float t, const T& y) {
			VectF::iterator it = std::upper_bound(m_nodes.begin(), m_nodes.end(), t);
			std::size_t d = std::distance(m_nodes.begin(), it);

			m_nodes.insert(it, t);
			m_pt.insert(m_pt.begin()+d, y);

			valMax_ = m_pt.back();

		    m_implSpline.GetCoef(m_nodes, m_pt, m_coef, loop_);

		}

		float getXmin() const {
			return m_nodes.front();
		}

		float getXmax() const {
			return m_nodes.back();
		}

        void setLoop(bool b) {
            loop_ = b;
        }

		T operator() (float tval) const {
			if( tval < m_nodes.front())
				return valMin_;
			else if(tval >= m_nodes.back())
				return valMax_;


			VectF::const_iterator it = std::upper_bound(m_nodes.begin(), m_nodes.end(), tval);
			//VectF::const_iterator it = std::lower_bound(m_nodes.begin(), m_nodes.end(), tval, std::less_equal<float>());
			--it;
			std::size_t d = std::distance(m_nodes.begin(), it);

			return eval(tval - *it, d);

		}


        T operator() (float tval, std::size_t &d) const {

            VectF::const_iterator it = m_nodes.begin() + d;
            while( *it < tval ) ++it;

            --it;

            d = std::distance(m_nodes.begin(), it);


            return eval(tval - *it, d);

        }


		void print() {
			std::cout << "Matrice Coef:" << std::endl;
			std::cout.precision(5);
			std::cout << std::fixed;
			typename VectT::const_iterator it = m_coef.begin();
			for(size_t i=0; i<m_nodes.size(); ++i, it += InterpImpl<T>::Deg) {
			    std::cout << m_nodes[i] << " : ";
			    for(int j=0; j<InterpImpl<T>::Deg; ++j)
                    std::cout << *(it+j) << '\t';
                it += InterpImpl<T>::Deg;
				std::cout << std::endl;
			}

		}

};

}

#endif
