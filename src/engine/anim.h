#ifndef ANIM_H
#define ANIM_H

#include <vector>
#include <SFML/Graphics.hpp>

class Anim
{
private:
	struct Frame {
		sf::Texture* texture;
		float dt;
	};

	typedef std::vector<Frame> VAnimFrame;
	VAnimFrame m_listeFrames;
	int iFrame, incFrame;
	float tCur;
	bool isEndQ, cycleQ;


public:
	Anim();
	~Anim();

	void addFrame(sf::Texture *t, float dt);

	void cycle(bool b) {
		cycleQ = b;
	}

	void begin();
	void goNext();
	void apply(sf::Sprite& sp) const {
		sp.setTexture(getTexture());
	}

	void update(float dt);
	sf::Texture& getTexture() const {
		return *(m_listeFrames[iFrame].texture);
	}

	float getRatio() const;
	void print() const;
};



#endif
