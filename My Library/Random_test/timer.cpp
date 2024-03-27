//ref: https://github.com/naskya/heuristic-contest/blob/main/src/main.cpp
struct timer {
    private:
    const std::chrono::system_clock::time_point start;

    public:
    static constexpr unsigned time_limit = 2000 - 20;

    timer() noexcept : start(std::chrono::system_clock::now()) {}

    [[nodiscard]] auto elapsed() const {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now() - start).count();
    }
    template <unsigned num, unsigned den>[[nodiscard]] bool frac() const {
        return elapsed() < time_limit * num / den;
    }
    [[nodiscard]] bool good() const {
        return elapsed() < time_limit;
    }
};