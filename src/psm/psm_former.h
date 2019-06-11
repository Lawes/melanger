#ifndef PSM_FORMER_TEMPLATE_HEADER
#define PSM_FORMER_TEMPLATE_HEADER

#include "psm_type.h"
#include "psm_particule.h"
#include "psm_common.h"
#include "psm_interp.h"


namespace PSM {

	template<
				template<class> class FadeInterpModel = IdentityInterp,
				template<class> class ColorInterpModel = IdentityInterp,
				template<class> class SizeInterpModel = IdentityInterp
			>
	class Former {
		public:
			typedef FadeInterpModel<float> FadeType;
			typedef ColorInterpModel<PSM::Color> ColorType;
			typedef SizeInterpModel<float> SizeType;

		private:
			FadeType m_fade_func;
			ColorType m_color_func;
			SizeType m_size_func;


		public:
			FadeType& getFade() { return m_fade_func; }
			ColorType& getColor() { return m_color_func; }
			SizeType& getSize() { return m_size_func;}

			void operator() (Particule& p, float lifeMax) const {
				float r = clamp(1.0f-p.vie/lifeMax, 0.0f, 1.0f);
				m_color_func(r, p.color);
				m_fade_func(r, p.color.a);
				m_size_func(r, p.taille);
			}

	};

}

#endif
