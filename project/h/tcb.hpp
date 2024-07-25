//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

// moji include-ovi
#include "list.hpp"
#include "sem.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() {
        delete stack;
    }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void *);

    static TCB *createThread(Body body, void* arg, uint64* stack_space);

    static void yield();

    static TCB *running;

    static void dispatch();

    // sto sam dodao
    static void join(TCB* tcbPtr);

    static int sleep(time_t time);

    static TCB * createThread_bez_schedulera(Body body, void* arg, uint64* stack_space);

private:
    TCB(Body body, void* arg, uint64 timeSlice) :
            body(body),
            arg(arg),
            context({(uint64) &threadWrapper,
                     (uint64) 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            //ono sto sam dodao
            waiting_to_join(nullptr)
//          sleeping(0)
    {}

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    // sto sam dodao

    TCB* waiting_to_join;
    static List<TCB> blockedOnJoin;
//    time_t sleeping;
//    static List<TCB> blockedOnSleep;


    friend class Riscv;

    //dodao sam
    friend class Sem;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);


    //    static uint64 timeSliceCounter;

    //static uint64 constexpr STACK_SIZE = 1024;
    //static uint64 constexpr TIME_SLICE = 5;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
