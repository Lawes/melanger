#ifndef PSM_INTERP_HEADER
#define PSM_INTERP_HEADER

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <limits>
#include <cmath>

#include "psm_common.h"

namespace PSM {


    template<class T>
	struct IdentityInterp {
		inline void operator() (const float r, T& val) const {

		}

	};

	template<class T>
	class LinearInterp {
		private:
			T m_pta, m_ptba;

		public:
			LinearInterp() : m_pta(), m_ptba() {}
			~LinearInterp() {}

			void setBornes(const T& a, const T& b) {
				m_pta = a;
				m_ptba = b - a;
			}

			inline void operator() (const float r, T& val) const {
				val = m_ptba*r + m_pta;
			}
	};

	template<class T>
	class DoubleLinearInterp {
		private:
			T m_pta, m_ptb, m_ptc;
			T m_seg1, m_seg2;
			float m_r0;

		public:
			DoubleLinearInterp() :  m_pta(), m_ptb(), m_ptc(), m_r0(0.5f) {}
			~DoubleLinearInterp() {}

			void setBornes(const T& a, const T& c) {
				m_pta = a;
				m_ptc = c;
			}
			void addPt(float r0, const T& b) {
				m_r0 = r0;
				m_ptb = b;
				m_seg1 = (m_ptb-m_pta)/m_r0;
				m_seg2 = (m_ptc-m_ptb)/(1.0f-m_r0);
			}

			void operator() (const float r, T& val) const {
				if( r < m_r0)
					val = m_seg1*r + m_pta;
				else
					val = m_seg2*(r-m_r0) + m_ptb;
			}
	};

    template <class T>
    class Interp_Segment {
        private:
            typedef std::vector< std::pair<T,T> > VectCoefs;

            std::vector<float> m_x;
            std::vector< T > m_pt;
            VectCoefs m_coefs;

            void computeCoef() {
                m_coefs.resize( m_x.size() - 1);
                for( std::size_t i=0; i<m_coefs.size(); ++i) {
                    m_coefs[i].first = m_pt[i];
                    if( std::abs(m_x[i+1] - m_x[i]) <= std::numeric_limits<float>::epsilon() )
                        m_coefs[i].second = T();
                    else
                        m_coefs[i].second = (m_pt[i+1] - m_pt[i])/(m_x[i+1] - m_x[i]);
                }
                m_coefs.back().first = m_pt.back();
                m_coefs.back().second *= 0.0f;
            }

        public:
            Interp_Segment() {
                reset();
            }
            virtual ~Interp_Segment() {};

            void reset(const T& val = T()) {
                m_x.resize(0);
                m_x.push_back(0.0f);
                m_x.push_back(1.0f);
                m_x.push_back(2.0f);

                m_pt.resize(3);
                std::fill(m_pt.begin(), m_pt.end(), val);
                computeCoef();
            }

            void setBornes(const T& a, const T& c) {
                m_pt.front() = a;
                m_pt.back() = c;
                m_pt[ m_pt.size() - 2] = c;
                computeCoef();

            }

            void addPt(float r, const T& val) {
                float rr = clamp(r, 0.0f, 1.0f);
				//lower bound ?
                typename std::vector< float >::iterator it = std::upper_bound(m_x.begin(), m_x.end(), rr);
                int d =it - m_x.begin();

                m_x.insert(it, rr);
                m_pt.insert(m_pt.begin() + d, val);

                computeCoef();
            }

            void operator() (float r, T& val) const {
				//lower bound ?
                typename std::vector< float >::const_iterator it = std::upper_bound(m_x.begin(), m_x.end(), r);
                --it;
                int d = it - m_x.begin();
                val = (r - *it) * m_coefs[d].second + m_coefs[d].first;

            }

            void print() {
                std::cout << "coef" << std::endl;
                for(std::size_t i=0; i<m_coefs.size(); ++i) {
                    std::cout << m_x[i] << ' ' << m_coefs[i].first << ' ' << m_coefs[i].second << ' ' << std::endl;

                }
            }
    };
}
#endif
