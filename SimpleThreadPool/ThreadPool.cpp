#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
    int threads = 10;

    for (int i = 0; i < threads; i++)
    {
        threadpool.push_back(std::thread(infiniteSpin, std::ref(*this)));
    }
}

ThreadPool::~ThreadPool()
{
    terminatePool = true;
    taskCondition.notify_all();
    for (std::thread& every_thread : threadpool)
    {
        every_thread.join();
    }

    threadpool.clear();
}


void ThreadPool::addTask(std::function<void()> t_task)
{
    {
        std::unique_lock<std::mutex> lock(taskMutex);
        tasks.push(t_task);
    }
    taskCondition.notify_all();
}

void ThreadPool::infiniteSpin(ThreadPool& t_threadpool)
{
    while (true)
    {
        std::function<void()> task = std::function<void()>();
        bool receivedTask = false;
        {
            std::unique_lock<std::mutex> lock(t_threadpool.taskMutex);
            t_threadpool.taskCondition.wait(lock, [&] {return !t_threadpool.tasks.empty() || t_threadpool.terminatePool; });
            if (!t_threadpool.terminatePool && !t_threadpool.tasks.empty())
            {
                task = t_threadpool.tasks.front();
                t_threadpool.tasks.pop();
                receivedTask = true;
            }
        }
        if (receivedTask)
        {
            task();
        }
    }
}