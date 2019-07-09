#ifndef GAME_CONFIG_HEADER
#define GAME_CONFIG_HEADER

#include <string>
#include <map>
#include <vector>

class GameConfig {
	public:
		struct Parameters {
			int width, height;
			float randtime;
			std::string imgfile, difficulty;
		};

		bool load(const char * filename);
		bool getParameters(const std::string& gamename, Parameters &p) const;

		std::vector<std::string> getSections() const {
			std::vector<std::string> v;
			for(auto it: m_content)
				v.push_back(it.first);
			return v;
		}
	private:
		std::map<std::string, Parameters> m_content;
};


#endif