#include "../h/syscall_cpp.hpp"

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}

//------------------------------------------------------------------

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
    thread_t pom = nullptr;
    thread_create_bez_schedulera(&pom, body, arg);
    this->myHandle = pom;
}

Thread::~Thread(){
    delete myHandle;
};

int Thread::start(){
    Scheduler::put(myHandle);
    return 0;
}

void Thread::join(){
    thread_join(myHandle);
}

void Thread::dispatch(){
    thread_dispatch();
}

//int Thread::sleep(time_t t){
//    return time_sleep(t);
//}

Thread::Thread(){
    thread_t pom = nullptr;
    thread_create_bez_schedulera(&pom, Thread::wrapper, this);
    this->myHandle = pom;
};


void Thread::wrapper(void* p){
    if(p!=0){
        ((Thread*) p)->run();
    }
}

//------------------------------------------------------------------


Semaphore::Semaphore(unsigned init){
    sem_t pom = nullptr;
    sem_open(&pom,init);
    this->myHandle = pom;
}

Semaphore::~Semaphore(){
    sem_close(myHandle);
}

int Semaphore::wait(){
    return sem_wait(myHandle);
}
int Semaphore::signal(){
    return sem_signal(myHandle);
}

//------------------------------------------------------------------


char Console::getc(){
    return ::getc();
}
void Console::putc(char c){
    ::putc(c);
}
