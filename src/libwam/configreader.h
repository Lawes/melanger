#ifndef CONFIG_DEVICE_READER_HEADER
#define CONFIG_DEVICE_READER_HEADER
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <utility>

#include "stringutils.h"

class ConfigReader {
    public:
        typedef std::list<std::string> Entries;
        typedef std::map<std::string, Entries> MapEntry;
        typedef std::map<std::string, MapEntry> MapTitle;

        void read(const char * fileConfig);
        void write(std::ostream &flux) const;

        void getTitles(Entries& liste) const;
        Entries getTitles() const;

        bool hasTitle(const std::string& titleName) const;

        template<class T>
        void add(const std::string& title, const std::string& clef,
                const T& value) {

            if( m_listeEntry.find(title) == m_listeEntry.end()) {
                m_listeEntry[title] = MapEntry();
            }

            MapEntry & me = m_listeEntry[title];
            auto it = me.find(clef);
            if( it == me.end()) {
                it = me.insert(std::make_pair(clef, Entries())).first;
            }

            it->second.push_back(std::to_string(value));
        }

        template<class Container>
        void get(const std::string& title, const std::string& clef,
                Container &out, bool first=false) const{
            typedef typename Container::value_type T;
            const auto it = m_listeEntry.find(title);
            if( it == m_listeEntry.end()) {
                std::cerr << "Erreur config reader: " << title << " not found!" << std::endl;
                return;
            }

            const MapEntry& tmpConf = it->second;
            const auto itt = tmpConf.find(clef);
            if( itt == tmpConf.end() ){
                std::cerr << "Erreur config reader: " << title << " : " << clef << " not found!" << std::endl;
                return;
            }
            for(const auto &item:itt->second) {
                out.push_back(from_string<T>(item));
                if( first) break;
            }
        }


        template<class T>
        T get(const std::string& title, const std::string& clef) const {
            std::list<T> tmp;
            get(title, clef, tmp, true);
            if(!tmp.empty()) 
                return tmp.front();
            std::cerr << "Erreur config reader: :" << title << ":" << clef << ": not found!" << std::endl;
            return T();
        }
    private:
        MapTitle m_listeEntry;
};

#endif
