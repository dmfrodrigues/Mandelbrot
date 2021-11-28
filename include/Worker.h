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
    void submit(Job *job){
        {
            std::lock_guard<std::mutex> lock(m);
            this->job = job;
            ready = true;
        }
        cv.notify_one();
    }
    void get(){
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]{ return !ready; });
        ready = false;
    }
private:
    void run(){
        while(true){
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this]{ return ready; });

            job->execute();
            ready = true;

            lock.unlock();
            cv.notify_one();
        }
    }
    std::thread t;
};
