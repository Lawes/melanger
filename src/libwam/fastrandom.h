#include <random>

//http://quick-bench.com/YGyEDXvfH1lyrm8oCNHCaRal24U

#define UNLIKELY(x) __builtin_expect((x), 0)

template<typename B, typename Rng>
void bench_sequential(benchmark::State& state) {
    Rng rng;
    B b;
    for (auto _ : state) {
        benchmark::DoNotOptimize(b(rng));
    }
}

// extremely fast random number generator that also produces very high quality random.
// see PractRand: http://pracrand.sourceforge.net/PractRand.txt
class sfc64 {
public:
    using result_type = uint64_t;

    static constexpr uint64_t(min)() { return 0; }
    static constexpr uint64_t(max)() { return UINT64_C(-1); }

    sfc64() : sfc64(std::random_device{}()) {}

    explicit sfc64(uint64_t seed) : m_a(seed), m_b(seed), m_c(seed), m_counter(1) {
        for (int i = 0; i < 12; ++i) {
            operator()();
        }
    }

    uint64_t operator()() noexcept {
        auto const tmp = m_a + m_b + m_counter++;
        m_a = m_b ^ (m_b >> right_shift);
        m_b = m_c + (m_c << left_shift);
        m_c = rotl(m_c, rotation) + tmp;
        return tmp;
    }

private:
    template <typename T> T rotl(T const x, int k) { return (x << k) | (x >> (8 * sizeof(T) - k)); }

    static constexpr int rotation = 24;
    static constexpr int right_shift = 11;
    static constexpr int left_shift = 3;
    uint64_t m_a;
    uint64_t m_b;
    uint64_t m_c;
    uint64_t m_counter;
};

class UniformDistribution {
  public:
    template <typename Rng> bool operator()(Rng &rng) {
        return std::uniform_int_distribution<>{0, 1}(rng);
    }
};

template <typename U = uint64_t> class BinaryAndUnbiased {
public:
    template <typename Rng> bool operator()(Rng &rng) {
        return (std::uniform_int_distribution<U>{}(rng)) & 1;
    }
};

template <typename U = uint64_t> class RandomizerWithCounter {
public:
    template <typename Rng> bool operator()(Rng &rng) {
        if (UNLIKELY(0 == m_counter)) {
            m_rand = std::uniform_int_distribution<U>{}(rng);
            m_counter = sizeof(m_rand) * 8;
        }
        return (m_rand >> --m_counter) & 1;
    }

private:
    U m_rand = 0;
    int m_counter = 0;
};

template <typename U = uint64_t> class RandomizerWithCounterMask {
public:
    template <typename Rng> bool operator()(Rng &rng) {
        auto b = m_counter & s_mask;
        if (UNLIKELY(0 == b)) {
            m_rand = std::uniform_int_distribution<U>{}(rng);
        }
        ++m_counter;
        return (m_rand >> b) & 1;
    }

private:
    static constexpr U s_mask = sizeof(U) * 8 - 1;
    U m_rand = 0;
    uint_fast8_t m_counter = 0;
};

template <typename U = uint64_t> class RandomizerWithSentinelShift {
public:
    template <typename Rng> bool operator()(Rng &rng) {
        if (UNLIKELY(1 == m_rand)) {
            m_rand = std::uniform_int_distribution<U>{}(rng) | s_mask_left1;
        }
        bool const ret = m_rand & 1;
        m_rand >>= 1;
        return ret;
    }

private:
    static constexpr const U s_mask_left1 = U(1) << (sizeof(U) * 8 - 1);
    U m_rand = 1;
};

template <typename Rng> class IndependentBitEngine {
public:
    template <typename RngUnused> bool operator()(RngUnused &rng) {
        return m_rng();
    }

private:
    std::independent_bits_engine<Rng, 1, bool> m_rng;
};



BENCHMARK_TEMPLATE(bench_sequential, UniformDistribution, std::mt19937);
//BENCHMARK_TEMPLATE(bench_sequential, BinaryAndUnbiased<>, std::mt19937);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithCounter<>, std::mt19937);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithCounterMask<>, std::mt19937);
BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithSentinelShift<>, std::mt19937);
BENCHMARK_TEMPLATE(bench_sequential, IndependentBitEngine<std::mt19937>, std::mt19937);

//BENCHMARK_TEMPLATE(bench_sequential, UniformDistribution, std::mt19937_64);
//BENCHMARK_TEMPLATE(bench_sequential, BinaryAndUnbiased<>, std::mt19937_64);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithCounter<>, std::mt19937_64);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithCounterMask<>, std::mt19937_64);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithSentinelShift<>, std::mt19937_64);

BENCHMARK_TEMPLATE(bench_sequential, UniformDistribution, sfc64); //3 the one to choose?
//BENCHMARK_TEMPLATE(bench_sequential, BinaryAndUnbiased<>, sfc64);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithCounter<>, sfc64);
//BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithCounterMask<>, sfc64);
BENCHMARK_TEMPLATE(bench_sequential, RandomizerWithSentinelShift<>, sfc64); //the fastest
BENCHMARK_TEMPLATE(bench_sequential, IndependentBitEngine<sfc64>, sfc64); //2
