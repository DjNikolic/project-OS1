//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"

#include "../test/printing.hpp"

extern void userMain();

int main()
{

    // kopirano telo funkcije iz riscv.hpp mc_sip samo sto mi treba za sie a ne za sip
    // ovim dvema instrukcijama sam maskirao prekide tajmera jer nisam uradio taj deo jos uvek
    __asm__ volatile ("csrc sie, %[mask]" : : [mask] "r"(Riscv::SSTATUS_SIE));
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    // definisanje prekidne rutine
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    // prebacujem proces u korisnicki rezim rada
    uint64 korisnickiReazim = 0x51;
    __asm__ volatile("mv a0, %[korisnickiReazim]" : : [korisnickiReazim] "r" (korisnickiReazim));  //upisujemo 0x51 u a0 registar
    __asm__ volatile("ecall");

    //pravim main nit i namestam program
    thread_t main_thread;
    thread_create(&main_thread, nullptr, nullptr);
    TCB::running = main_thread;
    printString("Main nit napravljena!\n");

     //poziv testa
    userMain();

    while (main_thread->isFinished() == false)
    {
        thread_dispatch();
    }

    delete main_thread;


    return 0;
}


