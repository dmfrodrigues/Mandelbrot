#include "Worker.h"

Worker::Worker() : t(run) {}

void Worker::submit(Job *job){
    {
        std::lock_guard<std::mutex> lock(m);
        this->job = job;
        ready = true;
    }
    cv.notify_one();
}

void Worker::get(){
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]{ return !ready; });
    ready = false;
}

void Worker::run(){
    while(true){
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]{ return ready; });

        job->execute();
        ready = true;

        lock.unlock();
        cv.notify_one();
    }
}
