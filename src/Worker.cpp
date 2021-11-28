#include "Worker.h"

Worker::Worker() : t([this]{ run(); }) {}

void Worker::submit(Job *job_){
    {
        std::lock_guard<std::mutex> lock(m);
        job = job_;
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
