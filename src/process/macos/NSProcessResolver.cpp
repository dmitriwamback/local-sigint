//
// Created by Dmitri on 2026-08-16.
//

#include "NSProcessResolver.h"

#include <vector>
#include <string>
#include <libproc.h>

std::vector<Process> NSProcessResolver::GetProcesses() {
    std::vector<Process> processes;

    int count = proc_listallpids(nullptr, 0);

    if (count <= 0) return processes;

    std::vector<pid_t> pids(count);

    int bytes = proc_listallpids(pids.data(), pids.size() * sizeof(pid_t));
    int actual = bytes / sizeof(pid_t);

    for (int i = 0; i < actual; i++) {
        pid_t pid = pids[i];

        if (pid <= 0) continue;

        Process process{};
        char processName[PROC_PIDPATHINFO_MAXSIZE];
        int length = proc_name(pid, processName, sizeof(processName));

        process.pid = pid;

        if (length > 0) {
            process.name = std::string(processName);
        }
        else continue;

        processes.push_back(process);
    }

    return processes;
}
