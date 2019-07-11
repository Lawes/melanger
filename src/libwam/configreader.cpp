#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "configreader.h"

using namespace std;

string whitespaces(" \t\f\v\n\r");

string rlstrip(const string& txt) {
    size_t first = txt.find_first_not_of(whitespaces);
    size_t last = txt.find_last_not_of(whitespaces);

    if( first == string::npos)
        return "";

    return txt.substr(first, last-first+1);

}


bool isTitle(const string& txt, string& title) {
    int res = false;
    if(  txt.compare(0,1,"@") == 0 ) {
        res = true;
        title = rlstrip(txt.substr(1, string::npos));
        if( title.empty() )
            res = false;
    }

    return res;

}


bool isAssignation(const string& txt, string& key, string& val) {
    size_t indice = txt.find_first_of('=');;
    bool res = false;

    if( indice != string::npos ) {
        res = true;
        key = rlstrip(txt.substr(0,indice));
        val = rlstrip(txt.substr(indice+1, string::npos));

        if( key.empty() || val.empty() )
            res = false;
    }

    return res;
}


bool ConfigReader::hasTitle(const std::string& titleName) const {
	MapTitle::const_iterator it = m_listeEntry.find(titleName);
	return it != m_listeEntry.end();

}


void ConfigReader::read(const char * fileConfig) {
    fstream flux(fileConfig, ios_base::in);

	if(!flux.good())
		return;

    string line_raw, key, val, title = "";

//    cout << "read file " << fileConfig << endl;

    while( !flux.eof()) {
        getline(flux, line_raw);
//        cout << "process: [" << line_raw << ']' << endl;
        size_t found = line_raw.find_first_of('#');
        string line = line_raw.substr(0, found);

        if( isTitle(line, title) ) {
            m_listeEntry[title] = MapEntry();
//            cout << "\ttitle=["<<title<<']'<<endl;
        }

        if( isAssignation(line, key, val) ) {
            if( title.empty() )
                break;
            MapEntry &me = m_listeEntry[title];
            auto it = me.find(key);
            if( it == me.end()) {
                it = me.insert(make_pair(key, list<string>())).first;
            }
            it->second.push_back(val);
        }
    }
    flux.close();
}

void ConfigReader::write(ostream& flux) const {
    for(const auto &etitle:m_listeEntry) {
        flux << '@' << etitle.first << endl;

        for(const auto &ekeys: etitle.second) {
            for( const auto &evalues: ekeys.second)
                flux << ekeys.first << " = " << evalues << endl;
        }

        flux << endl;
    }
}

void ConfigReader::getTitles(ConfigReader::Entries& liste) const {
    liste.clear();
    MapTitle::const_iterator it = m_listeEntry.begin();
    for(; it != m_listeEntry.end(); ++it)
        liste.push_back(it->first);
}

ConfigReader::Entries ConfigReader::getTitles() const {
    ConfigReader::Entries res;
    getTitles(res);
    return res;
}

