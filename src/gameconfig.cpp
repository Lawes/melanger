#include <fstream>

#include "gameconfig.h"
#include "configreader.h"
using namespace std;

void setParameters(ConfigReader &device, const string& name, GameConfig::Parameters& p) {
	p.width = device.get<int>(name, "width");
	p.height = device.get<int>(name, "height");
	p.imgfile = device.get<string>(name, "file");
	p.randtime = device.get<float>(name, "randtime");
	p.difficulty = device.get<string>(name, "difficulty");
}

bool GameConfig::load(const char * filename) {
	m_content.clear();
	ConfigReader device;
	device.read(filename);

	for(auto name:device.getTitles()) {
		Parameters p;
		setParameters(device, name, p);
		m_content.insert({name, p});
	}
	return true;
}

bool GameConfig::getParameters(const string& name, GameConfig::Parameters &p) const {
	const auto it = m_content.find(name);
	if( it == m_content.end())
		return false;
	
	p = it->second;
	return true;
}