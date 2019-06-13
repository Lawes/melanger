#include "ressourcemanager.h"
#include "configreader.h"

#include <list>

using namespace std;

bool RessourceManager::addAnimations(const string& filename) {
    ConfigReader device;
    device.read(filename.c_str());

    list<string> allanims;
    device.getTitles(allanims);

	cout << "reading animation" << endl;
    for( auto& anim: allanims) {
        cout << "name = " << anim << endl;
        auto a = MAnim::PtrType( new Anim());

        vector<string> frames;
        device.get(anim, "frame", frames);
        size_t nframes = frames.size();
        int cycle = device.get<int>(anim, "cycle");
        a->cycle(cycle==1);

        string textureName;
        float dt;
        sf::Texture *t=nullptr;
        for(auto& frame:frames) {
            itemize(frame, textureName, dt);
            cout << "frame: " << textureName << ", dt=" << dt << endl;
            addTexture(textureName, textureName);
            get(textureName, t);
            a->addFrame(t, dt);
        }
        a->print();
        m_anim.add(anim, std::move(a));
    }

    //device.write(cout);
    return true;
}