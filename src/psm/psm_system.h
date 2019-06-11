#ifndef PSM_SYSTEM_HEADER
#define PSM_SYSTEM_HEADER

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "psm_type.h"
#include "psm_systemBase.h"
#include "psm_former.h"

namespace PSM {

	template< class EmitterModel,
			  class MoverModel,
			  class RenderModel,
			  class FormerModel = Former<IdentityInterp,IdentityInterp,IdentityInterp>
			  >
	class ParticuleSystem : public ParticuleSystemBase, public sf::Drawable {
		private:
			std::vector<EmitterModel> emitters_;
			MoverModel mover_;
			RenderModel render_;
			FormerModel former_;

			float flow_, partLife_, partToCreate_;
			FuncVect m_endPartFunc;


		public:
            typedef typename std::vector<EmitterModel>::iterator EmitterIterator;

			ParticuleSystem(unsigned int n, float flow, float life):
				ParticuleSystemBase(n),
				emitters_(1),
				render_(n),
				flow_(flow),
				partLife_(life),
				partToCreate_(0.0f),
				m_endPartFunc([](PSM::Vector v){} ) 
			{
				for(auto &p: emitters_) p.setVie(partLife_);
			}

			virtual ~ParticuleSystem() {}

			EmitterModel& getEmitter(unsigned int i=0) { return emitters_[i]; }
			std::size_t getNbEmitters() const {
				return emitters_.size();
			}
			MoverModel& getMover() { return mover_; }
			RenderModel& getRender() { return render_; }
			FormerModel& getFormer() { return former_; }

			EmitterIterator beginEmitter() { return emitters_.begin(); }
			EmitterIterator endEmitter()  { return emitters_.end(); }

			void createEmitters(unsigned int n) {
				emitters_.resize(std::max(n,1u), emitters_[0]);
			}

			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
				render_(begin(), getNumPart());
				render_.draw(target, states);
			}

			virtual void update(const float dt) {
				ParticuleSystemBase::iterator it = begin();
				ParticuleSystemBase::iterator itend = end();

				while( it != itend) {
					if( mover_.updateParticule( *it, dt) ) {
					    m_endPartFunc(it->pos);
						--itend;
						unsigned int id = it->id;
						*it = *itend;
						itend->id = id;
					}
					else {
						former_(*it, partLife_);
						++it;
					}
				}

				setEnd(itend);

				if( !isActifQ() ) return;

				partToCreate_ += flow_ * dt;
				partToCreate_ -=  static_cast<float>(addParticules(partToCreate_));

			}


			int addParticules(float num) {
			    if(num <1.0f)
                    return 0;

				int intPart = getFreePart(num*static_cast<float>(emitters_.size()));

				ParticuleSystemBase::iterator it = end();
				EmitterIterator itEmitBegin = emitters_.begin();
				EmitterIterator itEmitEnd = emitters_.end();
				EmitterIterator itEmit = itEmitBegin;

				for(int i=0; i< intPart; i++, ++itEmit) {
					if( itEmit == itEmitEnd)
						itEmit = itEmitBegin;
                    if( itEmit->isPausedQ() ) continue;

					(*itEmit)(*it);
					//emitter_(*it);
					former_(*it, partLife_);
					++it;

				}
				setEnd(it);

				return static_cast<int>(num);
			}



			void createOneParticule() {
				ParticuleSystemBase::iterator it = end();
				if( it == max_end() ) return;
				emitters_[0](*it);
				former_(*it, partLife_);
				++it;
                setEnd(it);
			}

			void createParticule(PSM::Vector pos) {
				emitters_[0].setPos(pos);
				createOneParticule();
			}

			void endPartFunc(ParticuleSystemBase::FuncVect fun) {
			    m_endPartFunc = fun;
			}

	};

}



#endif
