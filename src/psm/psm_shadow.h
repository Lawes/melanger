#ifndef PSM_SHADOW_HEADER
#define PSM_SHADOW_HEADER

#include <vector>
#include <utility>

#include "psm_vector.h"

namespace PSM {

template<class PathModel>
class ShadowParticleSystem {
	private:
		std::size_t nShadowParticles;
		PathModel path_;
		std::vector<typename PathModel::TimeType> t_;
		std::vector<PSM::Vector> pos_;

	public:
		typedef typename std::vector<PSM::Vector>::iterator iterator;
		typedef typename std::vector<PSM::Vector>::const_iterator const_iterator;

		ShadowParticleSystem() {}
		ShadowParticleSystem(std::size_t n) :nShadowParticles(n), t_(n), pos_(n) { }
		~ShadowParticleSystem() {}

		PathModel& getPath() { return path_; }

		void setSize(std::size_t n) {
			nShadowParticles = n;
			t_.resize(n);
			pos_.resize(n);
		}

        std::size_t size() const {
            return nShadowParticles;
        }

		PSM::Vector getShadowPos(std::size_t i) const {
			return pos_[i];
		}

		void initStart(const std::vector<float>& v) {
			for(size_t i=0; i<nShadowParticles; ++i) {
				t_[i] = path_.getTime(v[i]);
			}
		}

		void update(const float dt) {
			for(size_t i=0; i<nShadowParticles; ++i) {
				PathModel::addTime(t_[i], dt);
				pos_[i] = path_(t_[i]);
			}
		}

};


}
#endif
