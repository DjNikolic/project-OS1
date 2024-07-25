#include "../h/handlers.hpp"


void* handler_mem_alloc(size_t size){
    return MemoryAllocator::getInstance().mem_alloc(size);
}

int handler_mem_free(void* curr){
    return MemoryAllocator::getInstance().mem_free(curr);
}

//-------------------------------------------------------------------------------------------

int handler_thread_create(
        thread_t* handle,
        void (*start_routine)(void*),
        void* arg
){
        uint64 sizeBlock = (DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE);
        if(DEFAULT_STACK_SIZE % MEM_BLOCK_SIZE){
            sizeBlock++;
        }
        uint64* stack_space = (uint64*)MemoryAllocator::getInstance().mem_alloc(sizeBlock);
        *handle = TCB::createThread(start_routine, arg, stack_space);
    return 0;
}

int handler_thread_exit(){
    TCB::running ->setFinished(true);
    TCB::dispatch();
    return 0;
}

void handler_thread_dispatch(){
    TCB::dispatch();
}

void handler_thread_join(
        thread_t handle
){
    TCB::join(handle);
}

//-------------------------------------------------------------------------------------------

int handler_sem_open(
        sem_t* handle,
        unsigned init
){
    *handle = Sem::createSem(init);
    return 0;
}

int handler_sem_close(sem_t handle){
    handle->close();
    return 0;
}

int handler_sem_wait(sem_t handle){
    int ret = handle->wait();
    return ret;
}

int handler_sem_signal(sem_t handle){
    handle->signal();
    return 0;
}

//-------------------------------------------------------------------------------------------

//int handler_time_sleep(
//        time_t time
//) {
//    return TCB::sleep(time);
//}

//-------------------------------------------------------------------------------------------

int handler_thread_create_bez_schedulera(
        thread_t* handle,
        void (*start_routine)(void*),
        void* arg
){
    uint64* stack_space = nullptr;
    if(start_routine != nullptr){
        uint64 sizeBlock = (DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE);
        if(DEFAULT_STACK_SIZE % MEM_BLOCK_SIZE){
            sizeBlock++;
        }
        stack_space = (uint64*)MemoryAllocator::getInstance().mem_alloc(sizeBlock);
    }
    *handle = TCB::createThread_bez_schedulera(start_routine, arg, stack_space);
    return 0;
}