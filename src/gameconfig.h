#ifndef GAME_CONFIG_HEADER
#define GAME_CONFIG_HEADER

#include <list>
#include <string>
#include <map>

class GameConfig {
	public:
		struct Parameters {
			int width, height, size;
			std::string imgfile;
		};

		bool load(const char * filename);
		bool getParameters(const std::string& gamename, Parameters &p) const;
	private:
		std::map<std::string, Parameters> m_content;
};


#endif