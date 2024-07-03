#pragma once
#include <iostream>
#include <chrono>
#include <thread>

class IETThread {
public:
	IETThread() = default;
	~IETThread() = default;

	void start() {
		std::thread(&IETThread::run, this).detach();
	}
	static void sleep(int ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

protected:
	virtual void run() = 0;
};