#ifndef TIMEIT_HEADER
#define TIMEIT_HEADER
#include <map>
#include <string>
#include <ostream>
#include <chrono>


class Timeit {
    public:
        typedef std::chrono::high_resolution_clock Clock;
        typedef Clock::time_point Time;
        typedef Clock::duration Delta;
        
        typedef std::chrono::nanoseconds nano;
        typedef std::chrono::milliseconds ms;
        typedef std::chrono::seconds sec;

        struct TimeCount {
            Delta dt;
            std::size_t count;
        };

        typedef std::map<std::string, TimeCount> Counter;

    private:
        std::size_t m_globalcount;
        Time m_tick;
        Counter m_counter;

    public:
        Timeit() : m_globalcount(0) {}
        ~Timeit() {}

        static Time getTime() {
            return Clock::now();
        }

        static double deltatime(const Time& t) {
            return to_sec(getTime()-t);
        }

        void reset();

        std::size_t increment() {
            return ++m_globalcount;
        }
        std::size_t getGlobalCount() const {
            return m_globalcount;
        }
        
        void tick() {
            m_tick = getTime();
        }

        Delta tock() const {
            return getTime()-m_tick;
        }

        Delta addTock(const std::string &key);

        void display(std::ostream& fout) const;

        static double to_ms(Delta dt) {
            return std::chrono::duration_cast<ms>(dt).count();
        }
        static double to_sec(Delta dt) {
            return std::chrono::duration_cast<sec>(dt).count();
        }

        static double to_nano(Delta dt) {
            return std::chrono::duration_cast<nano>(dt).count();            
        }

        template<class T=ms>
        double getMean(const std::string& key) const{
            auto it = m_counter.find(key);
            if( it == m_counter.end())
                return -1.0;
            const TimeCount &tc = it->second;
            if(tc.count == 0) return -1.0;
            double dt = static_cast<double>(std::chrono::duration_cast<T>(tc.dt).count());
            return dt/static_cast<double>(tc.count);
        }

        static std::string to_string(double t);

        Delta get(const std::string& key) const {
            auto it = m_counter.find(key);
            Delta res(0);
            if( it != m_counter.end())
                res = it->second.dt;
            return res;
        }
};


#endif