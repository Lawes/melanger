#ifndef MANAGER_HEADER
#define MANAGER_HEADER

#include <map>
#include <memory>
#include <string>
#include <iostream>

template<class T, typename ID = std::string>
class Manager {
    public:
        typedef std::unique_ptr<T> PtrType;
        typedef std::map<ID,  PtrType > TMap;
        typedef typename TMap::iterator iterator;
        typedef typename TMap::const_iterator const_iterator;
        virtual ~Manager() { clear();}

        bool add(const ID& id, PtrType item, bool force=false ) {
            auto it = m_liste.find(id);
            bool inliste = (it != m_liste.end()),
                 isadded = false;
            if(inliste && force) {
                m_liste.erase(it);
                m_liste[id] = std::move(item);
                isadded = true;
            }
            else if( !inliste) {
                m_liste[id] = std::move(item);
                isadded = true;
            }
            return isadded;
        }

        bool del(const ID& id) {
            auto it = m_liste.find(id);
            bool res = (it != m_liste.end());
            if (res) m_liste.erase(it);
            return res;
        }

        bool hasId(const ID& id) const {
            const_iterator it = m_liste.find(id);
            return (it != m_liste.end());
        }

        bool get(const ID& id, T& item) {
            if( !hasId(id))
                return false;
            item = *m_liste[id];
            return true;
        }

        T* get(const ID& id) {
            auto it = m_liste.find(id);
            if( it == m_liste.end()) {
                std::cerr << "Manager warning: " << id << " not found!" << std::endl;
                return nullptr;
            }

            return it->second.get();
        }

        iterator begin() { return m_liste.begin(); }
        iterator end() { return m_liste.end(); }
        const_iterator begin() const { return m_liste.begin();}
        const_iterator end() const { return m_liste.end();}

    private:
        TMap m_liste;
        void clear() {
            m_liste.clear();
        }
};

#endif
