#ifndef GUI_DECORATOR_HEADER
#define GUI_DECORATOR_HEADER

#include <map>
#include <string>
#include <functional>
#include "widget.h"

namespace gui {

    class DecoratorManager {
        public:
            typedef std::function< void(Widget&) > FuncWidget;
            typedef std::map<std::string, FuncWidget> DecoMap;

            bool apply(const std::string & id, Widget& w) const;
            void add(const std::string& id, FuncWidget func);
            bool hasId(const std::string& id) const;

            DecoratorManager() {}
            ~DecoratorManager() {}

        private:
            DecoMap m_content;

    };
}


#endif