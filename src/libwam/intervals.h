#ifndef INTERVALS_HEADER
#define INTERVALS_HEADER

#include <list>
#include <map>
#include <string>
#include <functional>
#include <memory>

class Interval {
    private:
        bool m_isFinished, m_inPause, m_loop;
    protected:
        float m_dt;

    public:
        typedef std::function<void(void)> FuncType;
        typedef std::unique_ptr<Interval> PtrInterval;

        Interval() : m_isFinished(false), m_inPause(false), m_loop(false), m_dt(0.0) {};
        virtual ~Interval();

        void start();
        void finish();
        void loop() { m_loop = true; }




        virtual bool isFinished() { return m_isFinished; }
        bool update(float dt);
        void pause() { m_inPause=true; }
        void resume() { m_inPause=false; }

    private:
        virtual bool _update() = 0;
        virtual void _start() {};
        virtual void _finish() {}

};

class IntervalAction : public Interval {
    private:
        FuncType m_execStart, m_execFinish;
        float m_timeElapsed, m_timeLimit;

    public:
        IntervalAction(float t);
        IntervalAction(FuncType f);

        void setStartFunc(FuncType f) {
            m_execStart = f;
        }

        void setFinishFunc(FuncType f) {
            m_execFinish = f;
        }

    private:
        virtual void _doAction(float p) { }
        virtual bool _update();
        virtual void _start();
        virtual void _finish();
};

typedef IntervalAction Wait;

class IntervalContainer : public Interval {
    protected:
        typedef std::list<PtrInterval> ListeInterval;
        typedef ListeInterval::iterator iterator;
        ListeInterval m_liste;
    public:
        ~IntervalContainer() { clear(); }
        void add(Interval *i);
        void clear();
    private:
        virtual bool _update() = 0;
};

class Sequence : public IntervalContainer {
    private:
        iterator m_itCurrent;
    public:
        Sequence() : m_itCurrent(m_liste.begin()) {}

        bool isFinished();
    private:
        void _finish();
        bool _update();
        void _start();
};

class Parallele : public IntervalContainer {
    public:
    private:
        void _finish();
        bool _update();
        void _start();
};


class Organizer {
    public:
        typedef int idType;
    private:
        typedef Interval::PtrInterval PtrInterval;
        typedef std::list<PtrInterval> ListInterval;
        typedef std::map<idType, PtrInterval> MapInterval;

        bool m_isRunning;
        ListInterval m_list;
        MapInterval m_map;

        void update_list(float dt);
        void update_map(float dt);

    public:
        Organizer() : m_isRunning(false) {}
        virtual ~Organizer() { }

        template<class T>
        T& add(T *interval) {
            if( m_isRunning) interval->start();
            m_list.push_back(PtrInterval(interval));
            return *interval;
        }

        template<class T>
        T& add(idType id, T *interval) {
            if( m_isRunning) interval->start();
            auto it = m_map.find(id);
            if( it != m_map.end())
                m_map.erase(it);
            m_map.insert({id, PtrInterval(interval)});
            return *interval;
        }

        void start();
        void finish();
        void update(float dt);
        void clear();
};

#endif
