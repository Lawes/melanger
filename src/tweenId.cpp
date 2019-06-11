#include "tweenId.h"
#include "timer.h"
#include <math.h>
#include <list>

namespace VODZO
{
	//TweenId::IdToTween TweenId::tweeners;

    bool Tweener::update() {
		timer += VODZO::deltaTime;
		if (timer > time) timer = time;
		float p = (timer/time);
		setValue(p);
	    return (timer >= time);
	}


	

	void TweenId::Update()
	{
        std::list<IdToTween::iterator> listeToRemove;

		for (IdToTween::iterator i = tweeners.begin(); i != tweeners.end(); ++i)
		{
			if( ((*i).second)->update() ) {
                listeToRemove.push_back(i);
            }
		}

		for (std::list<IdToTween::iterator>::iterator i = listeToRemove.begin(); i != listeToRemove.end(); ++i)
		{
            delete (*i)->second;
            tweeners.erase(*i);
        }
		
	}


	//clear all tweens
	void TweenId::Clear()
	{
		for (IdToTween::iterator i = tweeners.begin(); i != tweeners.end(); ++i)
		{
            delete i->second;
		}

		tweeners.clear();
	}




}
