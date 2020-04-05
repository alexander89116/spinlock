#include <atomic>

namespace NSpinLock {
    class TSpinLock {
    public:
        void lock();

        void unlock();

        bool try_lock();

    private:
        std::atomic_uint locked_{0};
    };
} // namespace NSpinLock