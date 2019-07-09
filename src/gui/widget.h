#ifndef WIDGET_HEADER
#define WIDGET_HEADER

#include <vector>
#include <list>
#include <string>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace gui {
    enum Alignement {
        HALIGN_LEFT,
        HALIGN_CENTER,
        HALIGN_RIGHT,
        VALIGN_TOP,
        VALIGN_CENTER,
        VALIGN_BOTTOM
    };

    class Widget: public sf::Drawable {
        public:
            typedef sf::FloatRect Box;
            typedef sf::RectangleShape Shape;
            typedef std::unique_ptr<Widget> PtrWidget;
            Widget();
            virtual ~Widget();

            //void encastre(const sf::FloatRect& box, float overlap);
            void enveloppe(const Box& box, float border);
            void aligneVertival(const Box& box, Alignement type, float e);
            void aligneHorizontal(const Box& box, Alignement type, float e);
            void juxtaposeVertival(const Box& box, Alignement type, float overlap);
            void juxtaposeHorizontal(const Box& box, Alignement type, float overlap);
            void center(const Box& box);
            void extend(float e);

            sf::Vector2f getCenter() const;

            bool contains(float x, float y) {
                return getBox().contains(x, y);
            }

            sf::Vector2f getPos() const {
                return sf::Vector2f(m_area.left, m_area.top);
            }

            Box& getBox() {
                return m_area;
            }

            Shape& getRenderBG() {
                return m_render_bg;
            }

            Shape& getRenderSelect() {
                return m_render_select;
            }

            sf::Text& getText() {
                return m_text;
            }

            void setText(const sf::String& txt, sf::Font& font, int size, bool force=false);
            void setText(const sf::String& txt, sf::Font& font, int size, sf::Color col, bool force=false);

            virtual void move(float x, float y);
            virtual void set_pos(float x, float y);
            virtual void set_size(float x, float y);

            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

            void select() {  m_isSelected = true; }
            void unSelect() { m_isSelected = false; }
            void setSelect(bool val) { m_isSelected=val;}
            bool getSelect() const { return m_isSelected;}

            void visible() { m_isVisible = true;}
            void unVisible() { m_isVisible = false;}
            void setVisible(bool val) { m_isVisible=val;}

			virtual bool find_widget(float x, float y, Widget& w) {
                w = *this;
                return true;
			}

            virtual void update();
            virtual void print() const;

        protected:
            Box m_area;
            Shape m_render_bg, m_render_select;
            sf::Text m_text;
            bool m_isSelected, m_isVisible, m_drawText, m_fixedsize;

        private:
            virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const {};

    };

    class Panel : public Widget {
        public:
            Panel(): Widget()  { }
            virtual ~Panel() {release();}

            virtual void add_child(Widget *w);

            virtual void release();
            virtual void move(float dx, float dy);
            virtual void set_pos(float pos_x, float pos_y);

            std::size_t getNWidgets() const {
                return m_widgets.size();
            }
			Widget& getWidget(size_t i) {
				return *m_widgets[i];
			}

			virtual bool find_widget(float x, float y, Widget& w);

            virtual void update();
            virtual void print() const;

        private:
            virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const;

        protected:
            typedef std::vector<PtrWidget> WidgetList;
            WidgetList m_widgets;

    };

    class BarPanel : public Panel {
        public:
            BarPanel(float espace=2, float border=10) : m_espace(espace), m_border(border)   {}
            virtual ~BarPanel() {};

            virtual void release();

            virtual void add_child(Widget *w, gui::Alignement al) = 0;
            void set_espace(float e) { m_espace = e;}
			void set_border(float b) { m_border = b; }

            virtual void set_pos(float pos_x, float pos_y);
            virtual void move(float dx, float dy);
            virtual void set_size(float x, float y);

            sf::FloatRect& getUseArea() {
                return m_use_area;
            }
            virtual void print() const;

        protected:
            sf::FloatRect m_use_area;
            float m_espace, m_border;
    };

    class HPanel : public BarPanel {
        public:
            HPanel(float espace=2, float border=10) : BarPanel(espace, border) {}

            virtual ~HPanel() {};
            virtual void add_child(Widget *w, gui::Alignement al = VALIGN_CENTER);
    };

    class VPanel : public BarPanel {
        public:
            VPanel(float espace=2, float border=10) : BarPanel(espace, border) {}
            virtual ~VPanel() {};
            virtual void add_child(Widget *w, Alignement al = HALIGN_CENTER);
    };

}
#endif
