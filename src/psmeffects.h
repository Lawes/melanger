#ifndef PSMEFFECTS_HEADER
#define PSMEFFECTS_HEADER

#include "gameobject.h"
#include "psm.h"



class ZoneObscure : public GameObject {
    private:
        PSM::ParticuleSystem<
            PSM::Explo,
            PSM::Vortex,
            PSM::render_SPRITES, PSM::Former<PSM::Interp_Segment > 
        > m_psm;

        PSM::ParticuleSystem<
            PSM::Explo,
            PSM::Vortex,
            PSM::render_LINES,
            PSM::Former<PSM::DoubleLinearInterp, PSM::LinearInterp>
        > m_psmline;

    public:
        ZoneObscure(sf::FloatRect rect);
        virtual ~ZoneObscure() {}
    DERIVED_GameObject;
};

class FeuxArtifice : public GameObject {
    private:
        PSM::ParticuleSystem<
            PSM::Emitter,
            PSM::Espace,
            PSM::render_SPRITES,
            PSM::Former<PSM::DoubleLinearInterp, PSM::LinearInterp, PSM::DoubleLinearInterp> 
        > m_fumefuse;

        PSM::ParticuleSystem<
            PSM::Explo,
            PSM::Earth,
            PSM::render_SPRITES,
            PSM::Former<PSM::DoubleLinearInterp, PSM::LinearInterp, PSM::DoubleLinearInterp> 
        > m_fumepart;

        PSM::ParticuleSystemAttached<
            PSM::Spray,
            PSM::Earth,
            PSM::render_SPRITES
        > m_fuse;

        PSM::ParticuleSystemAttached<
            PSM::Explo,
            PSM::Earth,
            PSM::render_SPRITES,
            PSM::Former<PSM::DoubleLinearInterp> 
        > m_part;

        std::vector<std::string> m_explosounds;

        void paf(PSM::Vector v);

    public:
        FeuxArtifice(sf::FloatRect rect);
        virtual ~FeuxArtifice() {}
    DERIVED_GameObject;
};

#endif