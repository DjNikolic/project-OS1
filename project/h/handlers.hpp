#ifndef _handlers
#define _handlers

#include "tcb.hpp"
#include "sem.hpp"
#include "MemoryAllocator.hpp"

typedef TCB _thread;
typedef _thread *thread_t;

typedef Sem _sem;
typedef _sem* sem_t;

//typedef unsigned long time_t;

extern "C" {

    //----------------------------------------

    void* handler_mem_alloc(size_t size);

    int handler_mem_free(void* curr);

    //----------------------------------------

    int handler_thread_create(
            thread_t *handle,
            void (*start_routine)(void *),
            void *arg
    );

    int handler_thread_exit();

    void handler_thread_dispatch();

    void handler_thread_join(
            thread_t handle
    );

    //----------------------------------------

    int handler_sem_open(
            sem_t* handle,
            unsigned init
    );

    int handler_sem_close(sem_t handle);

    int handler_sem_wait(sem_t handle);

    int handler_sem_signal(sem_t handle);

    //----------------------------------------

//    int handler_time_sleep(
//            time_t time
//    );

    //----------------------------------------

    int handler_thread_create_bez_schedulera(
            thread_t *handle,
            void (*start_routine)(void *),
            void *arg
    );

};

#endif