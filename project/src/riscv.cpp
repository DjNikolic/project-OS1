//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/handlers.hpp"
#include "../lib/mem.h"


void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 sstatus = r_sstatus();

    __asm volatile("csrw sscratch, a3");
    uint64 scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {

        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 pom;
        __asm__ volatile("mv %[pom], a0" : [pom] "=r" (pom));

        uint64 sepc = r_sepc() + 4;
        //uint64 volatile sstatus = r_sstatus();

        switch(pom){
            case 0x01 :  //mem_alloc
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("call handler_mem_alloc");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x02 :  //mem_free
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("call handler_mem_free");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x11 :  //thread_create
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("mv a1, a2");
                __asm__ volatile("csrr a2, sscratch");
                __asm__ volatile("call handler_thread_create");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x12 :  //thread_exit
                __asm__ volatile("call handler_thread_exit");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x13 :  //thread_dispatch
                __asm__ volatile("call handler_thread_dispatch");
                break;
            case 0x14 :  //thread_join
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("call handler_thread_join");
                break;
            case 0x31 :  //time_sleep
//                __asm__ volatile("mv a0, a1");
//                __asm__ volatile("call handler_time_sleep");
//                __asm volatile("csrw sscratch, a0");
//                break;
            case 0x21 :  //sem_open
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("mv a1, a2");
                __asm__ volatile("call handler_sem_open");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x22 : //sem_close
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("call handler_sem_close");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x23 :  //sem_wait
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("call handler_sem_wait");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x24 :  //sem_signal
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("mv a1, a2");
                __asm__ volatile("mv a2, a3");
                __asm__ volatile("call handler_sem_signal");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x41 :  //getc
                __asm__ volatile("call __getc");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x42 :  //putc
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("call __putc");
                break;
            case 0x50 :  //thread_create_bez_schedulera
                __asm__ volatile("mv a0, a1");
                __asm__ volatile("mv a1, a2");
                __asm__ volatile("csrr a2, sscratch");
                __asm__ volatile("call handler_thread_create_bez_schedulera");
                __asm volatile("csrw sscratch, a0");
                break;
            case 0x51 :  //korisnicki rezim rada
                // ovim prebacujemo bit 8 na vrednost 0 sto znaci da pri izlasku iz prekidne rutine prelazimo u korisnicki rezim
                sstatus = sstatus & ~(SSTATUS_SPP);
                break;
            default :
//                  TCB::timeSliceCounter = 0;
                TCB::dispatch();
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
//        TCB::timeSliceCounter++;

        // dodao sam moje
//        TCB* curr = TCB::blockedOnSleep.peekFirst();
//        TCB* prev;
//        while(curr != nullptr){
//            curr->sleeping--;
//            if(curr->sleeping == 0) {
//                Scheduler::put(curr);
//                prev = curr;
//                curr = TCB::blockedOnSleep.peekNextElem(curr);
//                TCB::blockedOnSleep.deleteElem(prev);
//            }else{
//                curr = TCB::blockedOnSleep.peekNextElem(curr);
//            }

//        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
//        {
//            uint64 volatile sepc = r_sepc();
//            uint64 volatile sstatus = r_sstatus();
//            TCB::timeSliceCounter = 0;
//            TCB::dispatch();
//            w_sstatus(sstatus);
//            w_sepc(sepc);
//        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }
}