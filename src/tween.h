#ifndef TWEENID_HEADER
#define TWEENID_HEADER

#include "libwam/intervals.h"

template< class T>
class TweenerLin : public IntervalAction {
    public:
        TweenerLin(T* value, T end, const float time) :
            IntervalAction(time), value_(value), start_(*value), end_(end) {}
        TweenerLin(T* value,T st, T end, const float time) :
            IntervalAction(time), value_(value), start_(st), end_(end) {}

    private:
        T *value_, start_, end_;
    private:
        virtual void _doAction(float p) { *value_ = ((end_-start_) * p) + start_; }
};

class TweenCollection : public Organizer {
public:
    TweenCollection() : Organizer() {}

    template <typename T>
    void To(int id, T *value, const T &end, float time) {
        add(id, new TweenerLin<T>(value, end, time));
    }

    template <typename T>
    void To(T *value, const T &end, float time) {
        add(new TweenerLin<T>(value, end, time));
    }

    template <typename T>
    void FromTo(int id, T *value, const T &start, const T &end, float time) {
        *value = start;
        To(id, value, end, time);
    }

    template <typename T>
    void FromTo(T *value, const T &start, const T &end, float time) {
        *value = start;
        To(value, end, time);
    }

    template <typename T>
    void To(int id, T *value, const T &end, float time, Interval::FuncType fun) {
        auto &t = add(id, new TweenerLin<T>(value, end, time));
        t.setFinishFunc(fun);
    }
    template <typename T>
    void To(T *value, const T &end, float time, Interval::FuncType fun) {
        auto &t = add(new TweenerLin<T>(value, end, time));
        t.setFinishFunc(fun);
    }
    
    template <typename T>
    void FromTo(int id, T *value, const T &start, const T &end, 
                        float time, Interval::FuncType fun) {
        *value = start;
        To(id, value, end, time, fun);
    }
    template <typename T>
    void FromTo(T *value, const T &start, const T &end, 
                        float time, Interval::FuncType fun) {
        *value = start;
        To(value, end, time, fun);
    }

};


#endif
