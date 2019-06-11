#ifndef PSM_SYSTEM_ATTACHED_HEADER
#define PSM_SYSTEM_ATTACHED_HEADER

#include "psm_system.h"
#include <functional>


namespace PSM {

	template< class EmitterModel,
			  class MoverModel,
			  class RenderModel,
			  class FormerModel = Former<IdentityInterp,IdentityInterp,IdentityInterp> >
	class ParticuleSystemAttached : public ParticuleSystem<EmitterModel,MoverModel,RenderModel, FormerModel>   {
		private:
			typedef std::function<void(PSM::Vector)> VectFunc;
			float m_refDt, m_timeDt;
			VectFunc m_funCreate;

		public:
            typedef ParticuleSystem<EmitterModel,MoverModel,RenderModel, FormerModel> ParticuleSystem_Model;

			ParticuleSystemAttached(int n, float flow, float life, float dt):
				ParticuleSystem_Model(n, flow, life),
				m_refDt(dt), m_timeDt(0.0f),
				m_funCreate([](PSM::Vector v){}) { }

			virtual ~ParticuleSystemAttached() {}

			void attachFunc(VectFunc func) {
			    m_funCreate = func;
			}

			void update(const float dt) {

				ParticuleSystem_Model::update(dt);

				m_timeDt += dt;
				if( m_timeDt > m_refDt) {
					m_timeDt = 0.0f;

					ParticuleSystemBase::iterator it = ParticuleSystemBase::begin();
					ParticuleSystemBase::iterator itend = ParticuleSystemBase::end();

					for(; it != itend; ++it) {
					    m_funCreate(it->pos);

					}
				}

			}


	};

}



#endif
