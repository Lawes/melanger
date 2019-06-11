#ifndef PSM_PATH_HEADER
#define PSM_PATH_HEADER

#include "psm_common.h"

#include "interpSpline.h"
#include "psm_type.h"
#include "vector2.h"

namespace PSM {
	template<template <class> class InterpModel>
	class Path {
		private:
			float maxTime;
			wam::Interp1<wam::Vector2, InterpModel> m_interp;

		public:
			typedef std::pair<float, std::size_t> TimeType;
			typedef InterpModel<wam::Vector2> InterpType;

			Path() : maxTime(0.0f) {}
			~Path() {}

			static void addTime(TimeType& timePt, float dt) {
				timePt.first += dt;
			}

			void init(const PSM::Vector& beginPt, float t, const PSM::Vector& endPt, bool loop = false) {

                m_interp.setLoop(loop);

				maxTime = t;

				std::vector<float> tdata;
				tdata.push_back(0.0f);
				tdata.push_back(t);

				std::vector< wam::Vector2> ydata;
				ydata.push_back(wam::Vector2(beginPt.x, beginPt.y));
				ydata.push_back(wam::Vector2(endPt.x, endPt.y));

                m_interp.init(tdata, ydata);
			}

			TimeType getTime(float t) {
				return std::make_pair(t*maxTime, 0u);
			}

			void addPt(float t, const PSM::Vector& pt) {
				m_interp.addPt(t, wam::Vector2(pt.x, pt.y));
			}

			PSM::Vector operator() (TimeType& timePt) {
				if( timePt.first >= maxTime) {
					timePt.first -= maxTime;
					timePt.second = 0u;
				}
				wam::Vector2 res(m_interp(timePt.first, timePt.second));
				return PSM::Vector(res.x, res.y);
			}

            void print() {
                m_interp.print();
            }

	};


    template<class CourbeModel>
    class Path_Param {
        private:
            float maxTime;
            CourbeModel courbe_;

        public:
			typedef float TimeType;

			Path_Param() : maxTime(0.0f) {}
			~Path_Param() {}

            CourbeModel& getCourbe() { return courbe_; }

			TimeType getTime(float t) {
				return t*maxTime;
			}

			static void addTime(TimeType& timePt, float dt) {
				timePt += dt;
			}

            void init(float t) {
                maxTime = t;
            }

			PSM::Vector operator() (TimeType& timePt) {
				if( timePt >= maxTime) timePt -= maxTime;

				return courbe_(timePt/maxTime);
/*
                float p = 180.0f * (timePt/maxTime - 0.5);
                float c = TableCS.Cos(p);
                float s = TableCS.Sin(p);
                float a = 4.0f;
				return PSM::Vector(a * c * c, a * c * s, a * std::sqrt((1.0f-c)*c));
				*/

			}
    };

}


#endif
