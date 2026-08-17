//
// Created by Dmitri on 2026-08-16.
//

#ifndef SIGINT_HOSTRESOLVER_H
#define SIGINT_HOSTRESOLVER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <atomic>
#include <thread>

#if defined(_WIN32)
#include <pcap.h>
#else
#include <pcap/pcap.h>
#endif

class DNSCapture {
public:
    using DNSCallback = std::function<void(const std::string& host)>;

    DNSCapture();
    ~DNSCapture();

    bool Start(DNSCallback callback);
    void Stop();

private:
    void CaptureLoop();
    std::string ParseDNSQuery(const unsigned char* packet, size_t packetLength);
    std::string ParseDNSName(const unsigned char* DNS, size_t DNSLength, size_t offset);

    pcap_t* handle = nullptr;

    std::thread captureThread;
    std::atomic<bool> running{false};

    DNSCallback callback;
};


#endif //SIGINT_HOSTRESOLVER_H
