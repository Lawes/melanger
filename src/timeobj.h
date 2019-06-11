#ifndef TIMEOBJ_HEADER
#define TIMEOBJ_HEADER

#include <utility>
#include <list>

template <class A>
class TimeObj {
    public:
        typedef std::pair<A,float> ObjType;
        typedef std::list<ObjType> ListeObjType;
        typedef typename ListeObjType::iterator iterator;
        typedef typename ListeObjType::const_iterator const_iterator;

        std::size_t size() const {
            return m_listeObj.size();
        }

        const A& getObj(std::size_t i) const {
            return m_listeObj[i].first;
        }

        iterator begin() { return m_listeObj.begin();}
        iterator end() { return m_listeObj.end();}
        const_iterator begin() const { return m_listeObj.begin();}
        const_iterator end() const { return m_listeObj.end();}

        void addObj(const A& obj, float t) {
            m_listeObj.emplace_back(obj, t);
        }

        void update(float dt) {
            iterator it = m_listeObj.begin(), itEnd=m_listeObj.end();
            while(it != itEnd) {
                it->second -= dt;
                if( it->second <= 0) {
                    m_listeObj.erase(it++);
                }
                else ++it;
            }
        }

    private:
        ListeObjType m_listeObj;
};

#endif