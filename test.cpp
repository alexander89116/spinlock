#include "spinlock/spinlock.hpp"
#include <thread>
#include <mutex>
#include <set>
#include <vector>

TEST(UnitTest, LockUnlock) {
    NSpinLock::TSpinLock spinlock;
    spinlock.lock();
    spinlock.unlock();
}

TEST(UnitTest, SequentialLockUnlock) {
    NSpinLock::TSpinLock spinlock;
    spinlock.lock();
    spinlock.unlock();
    spinlock.lock();
    spinlock.unlock();
}

TEST(UnitTest, TryLock) {
    NSpinLock::TSpinLock spinlock;
    ASSERT_TRUE(spinlock.try_lock());
    ASSERT_FALSE(spinlock.try_lock());
    spinlock.unlock();
    ASSERT_TRUE(spinlock.try_lock());
    spinlock.unlock();
    spinlock.lock();
    ASSERT_FALSE(spinlock.try_lock());
    spinlock.unlock();
}


TEST(StressTest, LockUnlock) {
    NSpinLock::TSpinLock spinlock;
    static const size_t kLockUnlockCount = 100000;
    for (size_t i = 0; i < kLockUnlockCount; i++) {
        spinlock.lock();
        spinlock.unlock();
    }
}

TEST(StressTest, TwoThreads) {
    NSpinLock::TSpinLock spinlock;
    int var = 0;
    std::thread thread1([&var, &spinlock]() {
        std::lock_guard<NSpinLock::TSpinLock> guard(spinlock);
        var++;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        ASSERT_TRUE(var == 1);
    });
    std::thread thread2([&var, &spinlock]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ASSERT_TRUE(var == 1);
        std::lock_guard<NSpinLock::TSpinLock> guard(spinlock);
        var++;
        ASSERT_TRUE(var == 2);
    });
    thread1.join();
    thread2.join();
    ASSERT_TRUE(var == 2);
}

TEST(StressTest, ManyThreads) {
    static const size_t kThreadsCount = 100;

    std::set<int> s;
    for (int i = 0; i < kThreadsCount; i++) {
        s.insert(i);
    }

    int var = 0;
    std::vector<std::thread> threads(kThreadsCount);
    NSpinLock::TSpinLock spinlock;

    for (int i = 0; i < kThreadsCount; i++) {
        threads[i] = std::thread([&var, &spinlock, &s]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::lock_guard<NSpinLock::TSpinLock> guard(spinlock);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            s.erase(var);
            var++;
        });
    }

    for (int i = 0; i < kThreadsCount; i++) {
        threads[i].join();
    }
    ASSERT_TRUE(var == kThreadsCount);
    ASSERT_TRUE(s.size() == 0);
}