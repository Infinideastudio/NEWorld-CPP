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
typedef std::function<void()> ThreadClenaupType;

enum class ThreadStatus { NotStarted, Preparing, Running, Paused, ExitedWithException };  // enum class ThreadStatus

class Thread final {
 public:
    Thread();
    Thread(const Thread &other) = delete;
    Thread(Thread &&other);
    Thread(ThreadPrepareType prepare, ThreadCallbackType loop, ThreadClenaupType cleanup, bool start = false);

    ~Thread() = default;

    Thread &operator=(const Thread &other) = delete;
    Thread &operator=(Thread &&other);

    /**
     * 启动线程
     */
    void Start();

    /**
     * 停止线程
     */
    void Stop();

    /**
     * 使暂停的线程继续
     */
    void Continue();

    /**
     * 暂停线程
     */
    void Pause();

    /**
     * 线程的准备函数
     * @return 指示准备工作是否成功
     */
    bool Prepare();

    /**
     * 线程的循环回调
     */
    void Loop();

    /**
     * 线程的清场函数
     */
    void Cleanup();

    /**
     * 等待线程准备完毕
     */
    void WaitForPrepare();

    /**
     * 等待线程结束
     */
    [[deprecated("Unecessary. Thread::Stop() can do the same thing.")]] void WaitForExit();

    /**
     * 返回线程当前状态
     * @return 状态值
     */
    ThreadStatus GetStatus();

 private:
    std::thread m_thread;

    std::atomic<bool> m_start;
    std::atomic<bool> m_pause;

    std::mutex m_mtx;
    std::condition_variable m_cv;

    ThreadPrepareType m_prepare;
    ThreadCallbackType m_loop;
    ThreadClenaupType m_cleanup;

    ThreadStatus m_status;
};  // class Thread

#endif  // NEWORLD_THREAD_HPP
