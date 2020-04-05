#include "spinlock.hpp"
#include "spinner.hpp"

namespace NSpinLock {
    void TSpinLock::lock() {
        TSpinner spinner;
        while (locked_.exchange(1)) {
            spinner();
        }
    }

    bool TSpinLock::try_lock() {
        return !(locked_.exchange(1));
    }

    void TSpinLock::unlock() {
        locked_.store(0);
    }
} // namespace NSpinLock