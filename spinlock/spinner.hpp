#include <thread>

namespace NSpinLock {
    class TSpinner {
        static const size_t kMaxSpinCount = 100;
    public:
        void operator()() noexcept {
            Spin();
        }

    private:
        void Spin() {
            spin_count_++;
            if (spin_count_ < kMaxSpinCount) {
                __asm __volatile("pause");
            } else {
                spin_count_ = 0;
                std::this_thread::yield();
            }
        }

    private:
        size_t spin_count_{0};
    };
} // namespace NSpinLock