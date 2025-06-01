// Copyright (C) 2025 Thomas R. Dial
#include "gdm.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace {

// This is the text message sent in a datagram when performing scans for
// Plex servers and clients.
const char* const kGdmMessage = "M-SEARCH * HTTP/1.0";

// The IP address constants used for the broadcast / multicast when
// scanning for clients and servers.
const char* const kGdmClientScanAddress = "255.255.255.255";
const char* const kGdmServerScanAddress = "239.0.0.250";

// The ports used for scanning clients, servers respectively.
const int kGdmClientScanPort = 32412;
const int kGdmServerScanPort = 32414;

// RAII wrapper that closes a file descriptor, unless dismissed.
class Closer {
 public:
  explicit Closer(int fd) : fd_(fd) {}
  ~Closer() {
    if (fd_ >= 0) {
      close(fd_);
    }
  }

  void Dismiss() { fd_ = -1; }

 private:
  int fd_;
};

// Create an unbound UDP socket.
// Returns valid socket if successful.
// Returns -1 on failure; check errno for reason.
int NewUdpSocket() {
  const int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  return s;
}

// Set the socket option for address reuse.
// Returns 0 if successful.
// Returns -1 on failure; check errno for reason.
int SetAddressReuseOption(int fd, bool reuse) {
  assert(fd >= 0);
  const int reuse_val = reuse ? 1 : 0;
  const int status =
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                 reinterpret_cast<void*>(reuse_val), sizeof(int));
  return status;
}

// Set the socket option for broadcast capability.
// Returns 0 if successful.
// Returns -1 on failure; check errno for reason.
int SetBroadcastOption(int fd, bool broadcast) {
  assert(fd >= 0);
  const int broadcast_value = broadcast ? 1 : 0;
  const int status =
      setsockopt(fd, SOL_SOCKET, SO_BROADCAST,
                 reinterpret_cast<void*>(broadcast_value), sizeof(int));
  return status;
}

// Set the Multicast TTL on the (UDP) socket.
// Returns 0 if successful.
// Returns -1 on failure; check errno for reason.
int SetMulticastTtlOption(int fd, int ttl) {
  assert(fd >= 0);
  assert(ttl >= 0);
  const int status = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL,
                                reinterpret_cast<void*>(&ttl), sizeof(int));
  return status;
}

}  // namespace

namespace plex {

GDM* GDM::New(GDM::ScanType type) {
  const int sock = NewUdpSocket();
  if (sock < 0) {
    return nullptr;
  }

  // RAII wrapper that closes the socket unless dismissed.
  Closer closer(sock);

  int status = 0;
  const char* ip_address = nullptr;
  unsigned short port = 0;
  switch (type) {
    case ScanType::kServer: {
      status = SetMulticastTtlOption(sock, 1);
      if (status < 0) {
        return nullptr;
      }
      ip_address = kGdmServerScanAddress;
      port = kGdmServerScanPort;
    } break;

    case ScanType::kClient: {
      status = SetAddressReuseOption(sock, true);
      if (status < 0) {
        return nullptr;
      }
      status = SetBroadcastOption(sock, true);
      if (status < 0) {
        return nullptr;
      }
      ip_address = kGdmClientScanAddress;
      port = kGdmClientScanPort;
    } break;

    default: {
      // Invalid ScanType
      return nullptr;
    }
  }

  // Parse the string IP to IPV4 address.
  struct in_addr inaddr{0};
  status = inet_aton(ip_address, &inaddr);
  if (status == 0) {
    // Invalid address.
    return nullptr;
  }

  const struct sockaddr_in addr{
      .sin_family = AF_INET, .sin_port = htons(port), .sin_addr{inaddr}};

  closer.Dismiss();
  return new GDM(sock, type, addr);
}

GDM::GDM(int fd, GDM::ScanType type, const struct sockaddr_in& address)
    : fd_(fd), scan_type_(type), address_(address) {
  assert(fd >= 0);
  assert(scan_type_ != ScanType::kNone);
}

int GDM::Scan() { return 0; }

GDM::~GDM() {
  assert(fd_ > 0);
  // Close socket.
  // TODO(tdial): Log error if close() fails.
  const int status = close(fd_);
  (void)status;
}

}  // namespace plex
