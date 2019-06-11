#ifndef INTERVALS_HEADER
#define INTERVALS_HEADER

#include <list>
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
        void loop() { m_loop = true; }
        virtual void finish() { m_isFinished = true; }
        virtual bool isFinished() { return m_isFinished; }
        bool update(float dt);
        void pause() { m_inPause=true; }
        void resume() { m_inPause=false; }

    private:
        virtual bool _update() = 0;
        virtual void _start() {};

};

class PopUp : public Interval {
    private:
        bool _update() {return false;}
};


class IntervalAction : public Interval {
    private:
        float m_timeElapsed, m_timeLimit, m_p;

    public:
        IntervalAction(const float t) : m_timeElapsed(0.0), m_timeLimit(t), m_p(0.0)  {};
        virtual void finish();

    private:
        virtual void _doAction(float p) = 0;
        virtual bool _update();
        virtual void _start();

};

class Wait : public IntervalAction {
    public:
        Wait(const float f) : IntervalAction(f) {}
    private:
        virtual void _doAction(float p)  {}
};

class SimpleFunc : public IntervalAction {
    public:
        SimpleFunc(Interval::FuncType fun) : IntervalAction(0.0), m_fun(fun) {}

    private:
        Interval::FuncType m_fun;
        virtual void _doAction(float p)  { m_fun(); }
};


template< class T>
class TweenerFunc : public IntervalAction {
    public:
        TweenerFunc(T* value, T end, const float time) :
            IntervalAction(time), value_(value), start(*value), end(end) {}
        TweenerFunc(T* value,T st, T end, const float time) :
            IntervalAction(time), value_(value), start(st), end(end) {}

    private:
        T *value_, start, end;
    private:
        virtual void _doAction(float p) { _doTwinnerAction(value_, start, end, p); }
        virtual void _doTwinnerAction(T* value,T st, T end, float p) = 0;
};

template< class T>
class TweenerLin : public TweenerFunc<T> {
    public:
        TweenerLin(T* value, T end, const float time) :
            TweenerFunc<T>(value, end, time) {}

        TweenerLin(T* value,T st, T end, const float time) :
            TweenerFunc<T>(value, st, end, time) {}
    private:
        virtual void _doTwinnerAction(T* value,T st, T end, float p) {
            *value = ((end-st) * p) + st;
        }
};


template< class T>
class TweenerSq : public TweenerFunc<T> {
    public:
        TweenerSq(T* value, T end, const float time) :
            TweenerFunc<T>(value, end, time) {}
        TweenerSq(T* value,T st, T end, const float time) :
            TweenerFunc<T>(value, st, end, time) {}

    private:
        virtual void _doTwinnerAction(T* value,T st, T end, float p) {
            *value = ((end-st) * p * p) + st;
        }

};

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
        void finish();
        bool isFinished();
    private:
        bool _update();
        void _start();
};

class Parallele : public IntervalContainer {
    public:
        void finish();
    private:
        bool _update();
        void _start();
};


class Organizer {
    private:
        typedef Interval::PtrInterval PtrInterval;
        typedef  std::list<PtrInterval> ListInterval;
        bool m_isRunning;
        ListInterval m_liste;

    public:
        Organizer() : m_isRunning(false) {}
        ~Organizer() { clear(); }
        void add(Interval *interval);
    // add avec id, a rappeler de temps en temps ?
    // add sans id (interval temporaire à supprimer directement
        void start();
        void update(float dt);
        void clear();
};

#endif
