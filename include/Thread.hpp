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

// 实现部分

Thread::Thread() : m_start(false), m_pause(false), m_status(ThreadStatus::NotStarted) {}

Thread::Thread(Thread &&other)
        : m_thread(std::move(other.m_thread))
        , m_start(false)
        , m_pause(false)
        , m_prepare(std::move(other.m_prepare))
        , m_loop(other.m_loop)
        , m_cleanup(other.m_cleanup)
        , m_status(ThreadStatus::NotStarted) {}

Thread::Thread(ThreadPrepareType prepare, ThreadCallbackType loop, ThreadClenaupType cleanup, bool start)
        : m_start(start), m_pause(false), m_prepare(prepare), m_loop(loop), m_cleanup(cleanup), m_status(ThreadStatus::NotStarted) {
    if (start) { Start(); }
}

Thread &Thread::operator=(Thread &&other) {
    m_thread = std::move(other.m_thread);
    m_start = m_pause = false;
    m_prepare = std::move(other.m_prepare);
    m_loop = std::move(other.m_loop);
    m_cleanup = std::move(other.m_cleanup);
    m_status = ThreadStatus::NotStarted;

    return *this;
}

void Thread::Start() {
    if (m_status == ThreadStatus::NotStarted || m_status == ThreadStatus::ExitedWithException) {
        m_start = true;
        m_status = ThreadStatus::Preparing;

        m_thread = std::thread([this]() {
            try {
                // 如果准备过程中发生错误，则立马停止
                // 并通知其他线程继续
                if (!this->Prepare()) {
                    this->m_status = ThreadStatus::ExitedWithException;
                    this->m_cv.notify_all();
                    return;
                }

                // 准备完成后通知所有正在等待的线程
                this->m_cv.notify_all();

                this->m_status = ThreadStatus::Running;
                while (this->m_start) {
                    while (!this->m_pause) { this->Loop(); }  // while

                    // 如果被暂停了会跳出上面的循环
                    this->m_status = ThreadStatus::Paused;

                    // 确认线程还未被停止
                    if (this->m_start) {
                        // 等待被继续
                        std::unique_lock<std::mutex> lock(this->m_mtx);
                        // 确保没有被暂停再继续
                        this->m_cv.wait(lock, [this]() { return !this->m_pause; });
                    }

                    this->m_status = ThreadStatus::Running;
                }  // while

                this->Cleanup();
                // 最后清理，方便下一次启动
                this->m_start = false;
                this->m_pause = false;

                this->m_status = ThreadStatus::NotStarted;
            } catch (...) {
                this->m_status = ThreadStatus::ExitedWithException;

                this->Cleanup();
                // 最后清理，方便下一次启动
                this->m_start = false;
                this->m_pause = false;
            }
        });  // lambda
    }
}

void Thread::Stop() {
    // 如果线程未启动就不鸟他
    if (m_status == ThreadStatus::NotStarted || m_status == ThreadStatus::ExitedWithException) { return; }

    m_start = false;

    // 如果已经被暂停了则先唤醒
    if (m_pause) { m_cv.notify_all(); } else {
        m_pause = true;
    }

    // 等待线程最终停止并销毁资源
    m_thread.join();
}

void Thread::Continue() {
    if (m_status == ThreadStatus::Paused) {
        m_pause = false;
        m_cv.notify_all();
    }
}

void Thread::Pause() {
    if (m_status != ThreadStatus::Paused && m_status == ThreadStatus::Running) { m_pause = true; }
}

bool Thread::Prepare() { return m_prepare(); }

void Thread::Loop() { m_loop(); }

void Thread::Cleanup() { m_cleanup(); }

void Thread::WaitForPrepare() {
    if (m_status == ThreadStatus::Preparing) {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cv.wait(lock, [this]() {return this->m_status != ThreadStatus::Preparing;});
    }
}


void Thread::WaitForExit() {
    m_thread.join();
}


ThreadStatus Thread::GetStatus() {
    return m_status;
}

#endif  // NEWORLD_THREAD_HPP
