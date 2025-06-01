// Copyright (C) 2025 Thomas R. Dial
#include "gdm.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace {

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

namespace plex {  //

GDM* GDM::New(GDM::ScanType type) {  //
  const int sock = NewUdpSocket();
  if (sock < 0) {
    return nullptr;
  }

  // RAII wrapper that closes the socket unless dismissed.
  Closer closer(sock);

  int status = 0;
  switch (type) {
    case ScanType::kServer: {
      status = SetMulticastTtlOption(sock, 1);
      if (status < 0) {
        return nullptr;
      }
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
    } break;

    default: {
      // Invalid ScanType
      return nullptr;
    }
  }

  return new GDM(sock, type);
}

GDM::GDM(int fd, GDM::ScanType type) {  //
}

GDM::~GDM() {  //
}

}  // namespace plex
