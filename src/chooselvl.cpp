#include "chooselvl.h"
#include "globals.h"


using namespace std;

ChooseLvl::ChooseLvl(SceneSwitcher *parent) : 
    Scene(parent),
    //m_zobs(m_context->getBox()),
    m_calme(m_context->getBox())
{ }


void ChooseLvl::_begin() {

    m_panel.release();
    clearEvents();

    sf::Font *font;
    RM.get("font", font);

    const auto& globalcfg= GB.getGlobalConfig();
    sf::Texture *texture;
    int count=1;
    gui::Widget *w;
    gui::HPanel *hp = new gui::HPanel(20,5);
    for( const auto& name: globalcfg.getSections()) {
        if(name[0] == '_')
            continue;
        cout << "lvl: " << name << endl;
        RM.get(name, texture);
        if( count%4 == 0) {
            m_panel.add_child(hp);
            hp = new gui::HPanel(20,5);
        }

        gui::VPanel *vp = new gui::VPanel(5,0);

        gui::HPanel *ww = new gui::HPanel(5,0);

        w = new gui::Widget();
        w->setText("> " + name, *font, 25, sf::Color::Green);
        ww->add_child(w);

        w = new gui::Widget();
        w->setText("("+GB.getConfig(name).difficulty+")", *font, 15, sf::Color::Cyan);
        ww->add_child(w);

        vp->add_child(ww);

        auto s = texture->getSize();
        float r = static_cast<float>(s.x)/s.y;
        w = new gui::Widget();
        w->set_size(170*r,170);
        w->getRenderBG().setFillColor(sf::Color::White);
        w->getRenderBG().setTexture(texture);
        vp->add_child(w);

        for(auto val: GB.getHighScore().getScore(name, 3, true)) {
            w = new gui::Widget();
            w->setText(to_string(val), *font, 15, sf::Color::White);
            vp->add_child(w);
        }
        DECOM.apply("box", *vp);
        add_event(vp->getBox(),  [vp]{ vp->select();}, [vp]{vp->unSelect();});
        add_event(
            sf::Mouse::Left,
            vp->getBox(),
            [this, name]{
                GB.beginRun(name);
                m_context->switchScene(scene::LaunchGame);
            }
        );

        hp->add_child(vp, gui::VALIGN_TOP);
        count++;
    }
    m_panel.add_child(hp);

    w = new gui::Widget();
    w->setText("Back to menu", *font, 25, sf::Color::White);
    DECOM.apply("txtbg", *w);
    DECOM.apply("underline", *w);
    add_event(w->getBox(), [w]{ w->select();}, [w]{w->unSelect();});
    add_event(
        sf::Mouse::Left,
        w->getBox(),
        [this]{
            m_context->switchScene(scene::GlobalPresentation);
        }
    ); 
    m_panel.add_child(w);

    m_panel.center(m_context->getBox());

    m_panel.update();

    //m_zobs.start();
    m_calme.start();
}

void ChooseLvl::_end() {
    //m_zobs.stop();
    m_calme.stop();

}

void ChooseLvl::update(float dt) {
    //m_zobs.update(dt);
    m_calme.update(dt);

}

void ChooseLvl::draw(sf::RenderWindow &win) const {
    //win.draw(m_zobs);
    win.draw(m_calme);
    win.draw(m_panel);
}

void ChooseLvl::load() {
    m_panel.set_espace(0);
    m_panel.set_border(0);
  
}