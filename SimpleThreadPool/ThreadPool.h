#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <queue>
#include <memory>
#include <condition_variable>

class ThreadPool 
{
public:
	ThreadPool();
	~ThreadPool();

	std::vector<std::thread> threadpool;
	void addTask(std::function<void()> task);
private:

	std::mutex taskMutex;
	std::queue<std::function<void()>> tasks;
	std::condition_variable taskCondition;

	bool terminatePool{ false };
	static void infiniteSpin(ThreadPool& tp);
};
