#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size){
    uint64 sizeBlock = (size / MEM_BLOCK_SIZE);
    if(size % MEM_BLOCK_SIZE){
        sizeBlock++;
    }
    __asm__ volatile("mv a1, %[sizeBlock]" : : [sizeBlock] "r" (sizeBlock));  //upisujemo sizeBlock u a1 registar
    uint64 sistemskiPoziv= 0x01;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x01 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return (void*)retVal;
}

int mem_free(void* ptr){
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x02;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x02 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

//-------------------------------------------------------------------------------------------------------------------------------

int thread_create(
        thread_t* handle,
        void (*start_routine)(void*),
        void* arg
        ){
    __asm__ volatile("mv a3, a2");
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x11;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x11 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

int thread_exit(){
    uint64 sistemskiPoziv= 0x12;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv)); //upisujemo 0x12 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

void thread_dispatch(){
    uint64 sistemskiPoziv= 0x13;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x13 u a0 registar
    __asm__ volatile("ecall");
}

void thread_join(
        thread_t handle
        ){
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x14;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x14 u a0 registar
    __asm__ volatile("ecall");
}

//-------------------------------------------------------------------------------------------------------------------------------

int sem_open(
        sem_t* handle,
        unsigned init
){
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x21;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x21 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

int sem_close(sem_t handle){
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x22;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x22 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

int sem_wait(sem_t handle){
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x23;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x23 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

int sem_signal(sem_t handle){
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x24;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x24 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}

//-------------------------------------------------------------------------------------------------------------------------------


//int time_sleep(time_t){
//    __asm__ volatile("mv a1, a0");
//    uint64 sistemskiPoziv= 0x31;
//    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x31 u a0 registar
//    __asm__ volatile("ecall");
//    uint64 retVal;
//    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
//    return retVal;
//}

//-------------------------------------------------------------------------------------------------------------------------------

char getc(){
    uint64 sistemskiPoziv= 0x41;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x41 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return (char)retVal;
}

void putc(char){
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x42;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x42 u a0 registar
    __asm__ volatile("ecall");
}

//-------------------------------------------------------------------------------------------------------------------------------


int thread_create_bez_schedulera(
        thread_t* handle,
        void (*start_routine)(void*),
        void* arg
){
    __asm__ volatile("mv a3, a2");
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");
    uint64 sistemskiPoziv= 0x50;
    __asm__ volatile("mv a0, %[sistemskiPoziv]" : : [sistemskiPoziv] "r" (sistemskiPoziv));  //upisujemo 0x50 u a0 registar
    __asm__ volatile("ecall");
    uint64 retVal;
    __asm__ volatile("mv %[retVal], a0" : [retVal] "=r" (retVal));  //uzimamo povratnu vrednost iz funkcije
    return retVal;
}
