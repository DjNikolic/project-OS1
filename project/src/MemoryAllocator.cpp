//
// Created by os on 8/12/23.
//
#include "../h/MemoryAllocator.hpp"

MemoryAllocator& MemoryAllocator::getInstance(){
    static MemoryAllocator instance;
    if(instance.first_enter == false){
        instance.free_mem_head = (MemStruct*)((uint8*)HEAP_START_ADDR);
        instance.free_mem_head->next = nullptr;
        instance.free_mem_head->prev = nullptr;
        instance.free_mem_head->size =(size_t)(((uint8*)HEAP_END_ADDR-(uint8*)HEAP_START_ADDR- 1)/MEM_BLOCK_SIZE);//broj raspolozivih blokova na pocetku
        instance.first_enter = true;
    }
    return instance;
}



//argument size je u blokovima a ne u bajtovima
void* MemoryAllocator::mem_alloc(size_t size){

    size_t needed_size_in_blocks = size + 1;

    for(MemStruct* curr = free_mem_head; curr != nullptr ; curr = curr->next) {

        if (curr->size >= needed_size_in_blocks) {

            uint8 *ret = ((uint8*)curr + MEM_BLOCK_SIZE);

            if (curr->size == needed_size_in_blocks) {
                //    ako se ne pravi element mem free
                if (curr->prev) {
                    curr->prev->next = curr->next;
                } else {
                    free_mem_head = curr->next;
                }
                if (curr->next) {
                    curr->next->prev = curr->prev;
                }
            }
            else {
                //    ako se pravi element mem free
                MemStruct* new_free_fragment = (MemStruct*) ((uint8*)curr +(needed_size_in_blocks * MEM_BLOCK_SIZE));
                if (curr->prev) {
                    curr->prev->next = new_free_fragment;
                } else {
                    free_mem_head = new_free_fragment;
                }
                if (curr->next) {
                    curr->next->prev = new_free_fragment;
                }
                new_free_fragment->prev = curr->prev;
                new_free_fragment->next = curr->next;
                new_free_fragment->size = curr->size - needed_size_in_blocks;
            }

            //treba da napravim element
            MemStruct* new_occupied_fragment = (MemStruct*) ((uint8*) curr);
            new_occupied_fragment->size = needed_size_in_blocks;  //broj zauzetih blokova

            if (occupied_mem_head == nullptr) {
                new_occupied_fragment->prev = nullptr;
                new_occupied_fragment->next = nullptr;
                occupied_mem_head = new_occupied_fragment;
            }
            else if (new_occupied_fragment < occupied_mem_head) {
                new_occupied_fragment->prev = nullptr;
                new_occupied_fragment->next = occupied_mem_head;
                occupied_mem_head->prev = new_occupied_fragment;
                occupied_mem_head = new_occupied_fragment;
            }
            else {
                MemStruct *pom = occupied_mem_head;
                for (; pom->next != nullptr && (uint8*)pom->next < (uint8*)new_occupied_fragment ;pom = pom->next);
                if (pom->next == nullptr) {
                    pom->next = new_occupied_fragment;
                    new_occupied_fragment->prev = pom;
                    new_occupied_fragment->next = nullptr;
                } else {
                    //   ovde ulazimo kada je pom->next > new_occupied_fragment && pom < new_occupied_fragment
                    pom->next->prev = new_occupied_fragment;
                    new_occupied_fragment->next = pom->next;
                    pom->next = new_occupied_fragment;
                    new_occupied_fragment->prev = pom;
                }
            }
            return ret;
        }
    }
    return nullptr;
}

void MemoryAllocator::tryToJoin(MemStruct* curr){
    if(curr == nullptr){
        return;
    }
    if(curr->next && ((uint8*)curr + (curr->size*MEM_BLOCK_SIZE)) == (uint8*)curr->next){
        curr->size +=curr->next->size;
        curr->next = curr->next->next;
        if(curr->next != 0){
            curr->next->prev = curr;
        }
    }
}

int  MemoryAllocator::mem_free(void* addr){

    MemStruct* curr_occupied = occupied_mem_head;
    for(; curr_occupied->next != nullptr && (uint8*)addr>(uint8*)(curr_occupied->next); curr_occupied = curr_occupied->next);
    if(curr_occupied == occupied_mem_head){
        if(curr_occupied->next == nullptr){
            occupied_mem_head = nullptr;
        }
        else{
            occupied_mem_head->next->prev = nullptr;
            occupied_mem_head = occupied_mem_head->next;
        }
    }
    else{
        if(curr_occupied->next == nullptr){
            curr_occupied->prev->next = nullptr;
        }
        else{
            curr_occupied->prev->next = curr_occupied->next;
            curr_occupied->next->prev = curr_occupied->prev;

        }
    }
    curr_occupied->prev = nullptr;
    curr_occupied->next = nullptr;
    //ovde imam curr_occupied sa vrednoscu size koji trebam prespojiti u free_mem


    MemStruct* curr_free;
    if(free_mem_head == nullptr  || (uint8*)addr < (uint8*) free_mem_head){
        curr_free = nullptr;
    }
    else{
        for(curr_free = free_mem_head; curr_free->next != nullptr && addr>(uint8*)(curr_free->next); curr_free = curr_free->next);
    }

//    if(curr_free == nullptr){
//        if(free_mem_head == nullptr){
//            free_mem_head = curr_occupied;
//        }
//        else{
//            curr_occupied->next = free_mem_head;
//            free_mem_head->prev = curr_occupied;
//            free_mem_head = curr_occupied;
//        }
//    }
//    else{
//        if(curr_free->next == nullptr){
//            curr_free->next = curr_occupied;
//            curr_occupied->prev = curr_free;
//        }
//        else{
//            curr_free->next->prev = curr_occupied;
//            curr_occupied->next = curr_free->next;
//            curr_free->next = curr_occupied;
//            curr_occupied->prev = curr_free;
//        }
//    }

    curr_occupied->prev = curr_free;
    if(curr_free != nullptr){
        curr_occupied->next = curr_free->next;
    }
    else{
        curr_occupied->next = free_mem_head;
    }
    if(curr_occupied->next){
        curr_occupied->next->prev = curr_occupied;
    }
    if(curr_free){
        curr_free->next = curr_occupied;
    }
    else{
        free_mem_head = curr_occupied;
    }

    tryToJoin(curr_occupied);
    tryToJoin(curr_free);

    return 0;
}













































