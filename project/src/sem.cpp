//
// Created by os on 8/10/23.
//

#include "../h/sem.hpp"

Sem* Sem::createSem(unsigned init){
    return new Sem(init);
}

void Sem::block(){
    TCB *old= TCB::running;
    this->blockedOnSem.addLast(old);
    TCB::running = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::running->context);
}

void Sem::deblock(){
    TCB* pom = this->blockedOnSem.removeFirst();
    Scheduler::put(pom);
}

int Sem::wait(){
    //maskiram prekide
    this->val--;
    if(this->val < 0){
        block();
        if(this->isAlive)
            return 0;
        else return -1;
    }
    return 0;
    //demaskiram prekide
}

void Sem::signal(){
    this->val++;
    if(this->val <=0){
        deblock();
    }
}

void Sem::close(){
    this->isAlive = false;
    TCB* curr = this->blockedOnSem.removeFirst();
    while(curr != nullptr){
        Scheduler::put(curr);
        curr = this->blockedOnSem.removeFirst();
    }
}