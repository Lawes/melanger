#ifndef PSM_SYSTEMBASE_HEADER
#define PSM_SYSTEMBASE_HEADER

#include <vector>
#include <functional>

#include "psm_particule.h"
#include "psm_type.h"

namespace PSM {

	class ParticuleSystemBase {
		public:
			typedef std::function<void(const PSM::Vector&)> FuncVect;
			typedef std::vector<PSM::Particule> AllParticule;
			typedef AllParticule::iterator iterator;
			typedef AllParticule::const_iterator const_iterator;

			ParticuleSystemBase(int n);
			virtual ~ParticuleSystemBase() {}

			virtual void update(const float dt) = 0;

			bool isActifQ() const {
				return actifQ_;
			}

			void start() {
				actifQ_ = true;
			}

			void stop() {
				actifQ_ = false;
			}

			void reset() {
				actifQ_ = false;
				firstInactif_ = 0;
			}

			int getNumPart() const {
				return firstInactif_;
			}

			void remove(iterator it);

			int getFreePart(float num) const;

			iterator get(int indice) {
				return pool_.begin()+indice;
			}

			iterator begin() {
				return pool_.begin();
			}

			const_iterator begin() const {
				return pool_.begin();
			}

			iterator end() {
				return pool_.begin() + firstInactif_;
			}

			const_iterator end() const {
				return pool_.begin() + firstInactif_;
			}

            iterator max_end() {
                return pool_.begin() + pool_.size();

            }

			void setEnd(iterator e) {
				firstInactif_ = std::distance( pool_.begin(), e);
			}

		private:
			bool actifQ_;
			std::vector<PSM::Particule> pool_;
			int firstInactif_;
	};
}

#endif