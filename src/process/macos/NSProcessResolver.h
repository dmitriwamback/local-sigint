//
// Created by Dmitri on 2026-08-16.
//

#ifndef SIGINT_NSPROCESSRESOLVER_H
#define SIGINT_NSPROCESSRESOLVER_H
#include <vector>
#include "../Process.h"

class NSProcessResolver {
public:
    static std::vector<Process> GetProcesses();
};


#endif //SIGINT_NSPROCESSRESOLVER_H
