//
// Created by os on 8/10/23.
//

#ifndef PROJECT_BASE_SEM_HPP
#define PROJECT_BASE_SEM_HPP

#include "tcb.hpp"
#include "list.hpp"

class Sem{
        public:

        static Sem* createSem(unsigned init);

        int wait();

        void signal();

        void close();

        private:
        int val;
        List<TCB> blockedOnSem;
        bool isAlive;

        Sem(unsigned init) :val(init), isAlive(true){}

        void block();

        void deblock();
};

#endif //PROJECT_BASE_SEM_HPP
