//
// Created by os on 8/12/23.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator{
public:
    static MemoryAllocator& getInstance();

    void* mem_alloc(size_t size);

    int  mem_free(void*);


private:
    struct MemStruct{
        MemStruct* next;
        MemStruct* prev;
        size_t size;
    };

    MemStruct* occupied_mem_head = nullptr;
    MemStruct* free_mem_head = nullptr;
    bool first_enter = false;

    MemoryAllocator() = default;
    ~MemoryAllocator() = default;

    void tryToJoin(MemStruct*);

};

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
