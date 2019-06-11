#include "ressourcemanager.h"
#include "configreader.h"
#include "configreader.h"

using namespace std;

bool RessourceManager::addTexture(const string& id, const string& filename) {
    auto ptr = MTexture::PtrType(new sf::Texture());
    bool res = ptr->loadFromFile(filename);
    if(res) m_texture.add(id, std::move(ptr));
    else
    {
        cerr << "unable to load texture : " << filename << " !" << endl;
    }
    
    return res;
}

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

bool RessourceManager::addFont(const string& id, const string& filename) {
    auto ptr = MFont::PtrType(new sf::Font());
    bool res = ptr->loadFromFile(filename);
    if(res) m_font.add(id, std::move(ptr));
    else
    {
        cerr << "unable to load font : " << filename << " !" << endl;
    }
    return res;
}

bool RessourceManager::addSound(const string& id, const string& filename) {
    auto ptr = MSound::PtrType(new sf::SoundBuffer());
    bool res = ptr->loadFromFile(filename);
    if(res) m_sound.add(id, std::move(ptr));
    else
    {
        cerr << "unable to load sound : " << filename << " !" << endl;
    }
    return res;    
}