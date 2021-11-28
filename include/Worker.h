#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>

#include "Job.h"

class Worker {
private:
    std::mutex m;
    std::condition_variable cv;

    bool ready = false;         ///@ True if the job is ready to be executed
    Job *job = nullptr;         ///@ Job to be executed
public:
    Worker();
    void submit(Job *job_);
    void get();
private:
    void run() __attribute__((noreturn));
    std::thread t;
};
