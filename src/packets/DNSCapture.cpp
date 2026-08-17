//
// Created by Dmitri on 2026-08-16.
//

#include "DNSCapture.h"

#include <cstring>
#include <cstdint>
#include <iostream>

inline uint16_t Read16(const unsigned char* packet) {
    return (static_cast<uint16_t>(packet[0]) << 8) | (static_cast<uint16_t>(packet[1]));
}

inline uint32_t Read32(const unsigned char* packet) {
    return (static_cast<uint32_t>(packet[0]) << 24) | (static_cast<uint32_t>(packet[1]) << 16) | (static_cast<uint32_t>(packet[2]) << 8) | static_cast<uint32_t>(packet[3]);
}

DNSCapture::DNSCapture() = default;

DNSCapture::~DNSCapture() {
    Stop();
}

bool DNSCapture::Start(DNSCallback callback) {
    if (running) return false;

    this->callback = std::move(callback);
    char errbuf[PCAP_ERRBUF_SIZE]{};

    pcap_if_t* devices = nullptr;

    if (pcap_findalldevs(&devices, errbuf) == -1) {
        std::cerr << errbuf << std::endl;
        return false;
    }

    if (!devices) {
        std::cerr << "Np capture interface found" << std::endl;
        return false;
    }

    pcap_if_t* selectedDevice = nullptr;

    for (pcap_if_t* device = devices; device != nullptr; device = device->next) {
        if (std::string(device->name) == "en0") {
            selectedDevice = device;
        }
    }

    if (selectedDevice == nullptr) {
        std::cerr << "No en0 capture interface found" << std::endl;
        pcap_freealldevs(devices);
        return false;
    }

    handle = pcap_create(selectedDevice->name, errbuf);

    if (!handle) {
        std::cerr << errbuf << std::endl;
        return false;
    }

    if (pcap_set_snaplen(handle, 65535) != 0) {
        std::cerr << "Failed to set snaplen" << std::endl;
        pcap_close(handle);
        handle = nullptr;
        pcap_freealldevs(devices);
        return false;
    }

    if (pcap_set_promisc(handle, 0) != 0) {
        std::cerr << "Failed to set promisc" << std::endl;
        pcap_close(handle);
        handle = nullptr;
        pcap_freealldevs(devices);
        return false;
    }

    if (pcap_set_timeout(handle, 1000) != 0) {
        std::cerr << "Failed to set timeout" << std::endl;
        pcap_close(handle);
        handle = nullptr;
        pcap_freealldevs(devices);
        return false;
    }

    int result = pcap_activate(handle);
    if (result != 0) {
        std::cerr << "Failed to activate " << selectedDevice->name << std::endl;
        std::cerr << "result: " << result << std::endl;
        std::cerr << "pcap error: " << pcap_geterr(handle) << std::endl;
        pcap_close(handle);
        handle = nullptr;
        pcap_freealldevs(devices);
        return false;
    }

    bpf_program filter{};
    const char* filterExpression = "udp port 53";

    if (pcap_compile(handle, &filter, filterExpression, 1, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Failed to compile filter: " << pcap_geterr(handle) << std::endl;
        pcap_close(handle);
        handle = nullptr;
        pcap_freealldevs(devices);
        return false;
    }

    if (pcap_setfilter(handle, &filter) == -1) {
        std::cerr << "Failed to set filter" << pcap_geterr(handle) << std::endl;
        pcap_close(handle);
        handle = nullptr;
        pcap_freealldevs(devices);
        return false;
    }
    pcap_freecode(&filter);
    pcap_freealldevs(devices);
    running = true;
    captureThread = std::thread(&DNSCapture::CaptureLoop, this);

    return true;
}

void DNSCapture::Stop() {
    if (!running) return;

    running = false;
    if (handle) pcap_breakloop(handle);

    if (captureThread.joinable()) captureThread.join();

    if (handle) {
        pcap_close(handle);
        handle = nullptr;
    }
}

void DNSCapture::CaptureLoop() {
    while (running) {

        struct pcap_pkthdr* header = nullptr;
        const unsigned char* packet = nullptr;

        int result = pcap_next_ex(handle, &header, &packet);

        if (result == 1) {
            if (header == nullptr || packet == nullptr) {
                std::cout << "NULL packet/header" << std::endl;
                continue;
            }

            std::cout << "packet address: " << static_cast<const void*>(packet) << std::endl;
            std::cout << "packet length: " << header->caplen << std::endl;

            std::string hostname = ParseDNSQuery(packet, header->caplen);

            if (!hostname.empty() && callback) {
                callback(hostname);
            }
        }
        else if (result == 0) continue;
        else {
            std::cerr << pcap_geterr(handle) << std::endl;
            break;
        }
    }
}

std::string DNSCapture::ParseDNSQuery(const unsigned char* packet, size_t packetLength) {

    if (packetLength < 14) return {};

    uint16_t etherType = Read16(packet + 12);

    std::cout << "EtherType: 0x"
          << std::hex
          << etherType
          << std::dec
          << std::endl;

    size_t IPOffset = 14;
    size_t UDPOffset = 0;

    if (etherType == 0x0800) {

        if (packetLength < IPOffset + 20) return {};

        unsigned char versionIhl = packet[IPOffset];
        unsigned char version = versionIhl >> 4;
        unsigned char ihl = versionIhl & 0x0F;

        if (version != 4) return {};

        size_t ipHeaderLength = ihl * 4;

        if (ipHeaderLength < 20) return {};
        if (packetLength < IPOffset + ipHeaderLength) return {};

        unsigned char protocol = packet[IPOffset + 9];

        if (protocol != 17) return {};

        UDPOffset = IPOffset + ipHeaderLength;
    }
    else if (etherType == 0x86DD) {

        if (packetLength < IPOffset + 40) return {};
        unsigned char version = packet[IPOffset] >> 4;

        if (version != 6) return {};
        unsigned char nextHeader = packet[IPOffset + 6];

        if (nextHeader != 17) return {};
        UDPOffset = IPOffset + 40;
    }


    if (packetLength < UDPOffset + 8) return {};
    uint16_t sourcePort = Read16(packet + UDPOffset);
    uint16_t destinationPort = Read16(packet + UDPOffset + 2);

    if (sourcePort != 53 && destinationPort != 53) return {};

    size_t DNSOffset = UDPOffset + 8;
    if (packetLength < DNSOffset + 12) return {};

    size_t DNSLength = packetLength - DNSOffset;
    const unsigned char* DNS = packet + DNSOffset;

    uint16_t flags = Read16(DNS + 2);
    bool isResponse = (flags & 0x8000) != 0;

    if (isResponse) return {};

    uint16_t questions = Read16(DNS + 4);
    if (questions == 0) return {};

    return ParseDNSName(DNS, DNSLength, 12);
}

std::string DNSCapture::ParseDNSName(const unsigned char* DNS, size_t DNSLength, size_t offset) {
    std::string hostname;
    size_t current = offset;

    size_t iterations = 0;
    while (current < DNSLength && iterations++ < 128) {
        unsigned char labelLength = DNS[current];
        if (labelLength == 0) break;

        if ((labelLength & 0xC0) == 0xC0) return {};
        if (labelLength > 63) return {};

        current++;

        if (current + labelLength > DNSLength) return {};
        if (!hostname.empty()) hostname += '.';

        for (size_t i = 0; i < labelLength; i++) {
            unsigned char c = DNS[current + i];

            if (c < 0x21 || c > 0x7E) return {};
            hostname += static_cast<char>(c);
        }
        current += labelLength;
    }
    std::cout << "DNS Name: " << hostname << std::endl;
    return hostname;
}