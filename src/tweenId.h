#ifndef TWEENID_HEADER
#define TWEENID_HEADER

//todo: replace list with something more portable
#include <map>
#include "Vector2.h"
#include "functor.h"


namespace VODZO
{

    class Tweener {
        public:
            Tweener(const float t) : timer(0.0f), time(t) {};

            virtual bool update();
            virtual void setValue(const float p) = 0;

        private:
            float timer, time;
    };


    template< typename T>
    class TweenerType : public Tweener {
        public:
            TweenerType(T* value, T end, const float time) : Tweener(time), value_(value), start(*value), end(end) {}

            virtual void setValue(float p) {
                *value_ = ((end-start) * p) + start;
            }

        private:
            T *value_, start, end;
    };

    template< typename T>
    class TweenerTypeFun : public TweenerType<T> {
        public:
            TweenerTypeFun(T* value, T end, const float time, Functor_v& fun) 
                : TweenerType<T>(value, end, time), fun_(fun) {}

            bool update() {
                bool res = Tweener::update();
                if( res)
                    fun_();
                return res;
            }

        private:
            Functor_v fun_;
    };

	class TweenId
	{
	public:
        typedef std::map<int, Tweener*> IdToTween;

		TweenId() {}
        ~TweenId() { Clear(); }

		template <typename T>
		void To(int id, T *value, const T &end, float time) {
            IdToTween::iterator it = tweeners.find(id);
            if( it != tweeners.end()) {
                delete (*it).second;
            }
            Tweener *t = (Tweener*)new TweenerType<T>(value, end, time);
            tweeners[id] = t;
        }



		template <typename T>
		void FromTo(int id, T *value, const T &start, const T &end, float time) {
            *value = start;
            To(id, value, end, time);
        }


		template <typename T>
		void To(int id, T *value, const T &end, float time, Functor_v& fun) {
            IdToTween::iterator it = tweeners.find(id);
            if( it != tweeners.end()) {
                delete (*it).second;
            }
            Tweener *t = (Tweener*)new TweenerTypeFun<T>(value, end, time, fun);
            tweeners[id] = t;
        }
        
		template <typename T>
		void FromTo(int id, T *value, const T &start, const T &end, 
                           float time, Functor_v& fun) {
            *value = start;
            ToF(id, value, end, time, fun);
        }


		void Update();
		void Clear();

	private:
		IdToTween tweeners;

	};


}
#endif
