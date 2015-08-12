//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_THREAD_HPP
#define NEWORLD_THREAD_HPP

#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>

typedef std::function<bool()> ThreadPrepareType;
typedef std::function<void()> ThreadCallbackType;

enum class ThreadStatus {
    NotStarted,
    Preparing,
    Running,
    Paused,
    ExitedWithException
};  // enum class ThreadStatus


class Thread final {
public:
    Thread() = delete;
    Thread(const Thread &other) = delete;
    Thread(Thread &&other) = delete;
    Thread(
        ThreadPrepareType prepare,
        ThreadCallbackType loop,
        ThreadCallbackType cleanup,
        bool start = false);
    ~Thread() = default;

    Thread &operator=(const Thread &other) = delete;
    Thread &operator=(Thread &&other) = delete;

    void Start();
    void Stop();
    void Continue();
    void Pause();

    bool Prepare();
    void Loop();
    void Cleanup();

    void WaitForPrepare();

    [[deprecated("Unecessary. Thread::Stop() can do the same thing.")]]
    void WaitForExit();

    ThreadStatus GetStatus();

private:
    std::thread m_thread;

    std::atomic<bool> m_start;
    std::atomic<bool> m_pause;

    std::mutex m_mtx;
    std::condition_variable m_cv;

    ThreadPrepareType m_prepare;
    ThreadCallbackType m_loop;
    ThreadCallbackType m_cleanup;

    ThreadStatus m_status;
};  // class Thread

#endif  // NEWORLD_THREAD_HPP
