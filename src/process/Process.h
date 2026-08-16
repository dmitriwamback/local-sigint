//
// Created by Dmitri on 2026-08-16.
//

#ifndef SIGINT_PROCESS_H
#define SIGINT_PROCESS_H

#include <string>

struct Process {
    uint32_t pid;
    std::string name;
    std::string path;
    double CPUUsage;
    uint64_t memory;
};

#endif //SIGINT_PROCESS_H
