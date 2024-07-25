//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

//uint64 TCB::timeSliceCounter = 2;


TCB *TCB::createThread(Body body, void* arg, uint64* stack_space)
{
    TCB* ret =  new TCB(body, arg, DEFAULT_TIME_SLICE);
    ret->stack = stack_space;
    ret->context.sp = (uint64) &ret->stack[DEFAULT_STACK_SIZE];
    if (body != nullptr) {
        Scheduler::put(ret);
    }return ret;
}

void TCB::yield()
{
    uint64 sistemskiPoziv= 0x00;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && old->waiting_to_join == nullptr) {
        // mogu da sacuvam sstatus zbog ecall-a
        Scheduler::put(old);
        // ovde da ga odsacuvam
    }if (old->isFinished()){
        TCB* curr = TCB::blockedOnJoin.peekFirst();
        TCB* prev;
        while(curr != nullptr){
            if(curr->waiting_to_join == running){
                curr->waiting_to_join = nullptr;
                Scheduler::put(curr);
                prev = curr;
                curr = TCB::blockedOnJoin.peekNextElem(curr);
                TCB::blockedOnJoin.deleteElem(prev);
            }else {
                curr = TCB::blockedOnJoin.peekNextElem(curr);
            }
        }
    }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}

// ono sto sam dodao

List<TCB> TCB::blockedOnJoin;

void TCB::join(TCB* tcbPtr){
    if(tcbPtr->isFinished() == true)
        return;
    running->waiting_to_join = tcbPtr;
    blockedOnJoin.addLast(TCB::running);
    TCB::dispatch();
}

//List<TCB> TCB::blockedOnSleep;

//int TCB::sleep(time_t time){
//    TCB *old = running;
//    TCB *next = Scheduler::get();
//    if(next == 0){
//        return -1;
//    }else{
//        old->sleeping = time;
//        blockedOnSleep.addLast(old);
//        running = next;
//        TCB::contextSwitch(&old->context, &next->context);
//        return 0;
//    }
//}

TCB *TCB::createThread_bez_schedulera(Body body, void* arg, uint64* stack_space){
    TCB* ret = new TCB(body, arg, DEFAULT_TIME_SLICE);
    ret->stack = stack_space;
    if(stack_space != nullptr){
        ret->context.sp = (uint64) &ret->stack[DEFAULT_STACK_SIZE];
    }else{
        ret->context.sp = 0;
    }
    return ret;
}

