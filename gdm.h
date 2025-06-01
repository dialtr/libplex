// Copyright (C) 2025 Thomas R. Dial
#ifndef GDM_H_
#define GDM_H_

#include <arpa/inet.h>

namespace plex {

class GDM {
 public:
  enum class ScanType {
    kNone = 0,    // Undefined scan type.
    kServer = 1,  // Scan for servers.
    kClient = 2   // Scan for clients.
  };

  struct Options {
    GDM::ScanType type = ScanType::kNone;
    const char* message = nullptr;
    const char* address = nullptr;
    unsigned short port = 0;
  };

  // Create new instance.
  static GDM* New(const Options& options);

  // Scan for plex hosts.
  int Scan();

  // Destroy the instance.
  ~GDM();

 private:
  GDM(int fd, GDM::ScanType type, const struct sockaddr_in& address);

  int fd_ = -1;
  GDM::ScanType scan_type_ = ScanType::kNone;
  const struct sockaddr_in address_{0};
  unsigned short port_ = 0;
};

}  // namespace plex

#endif  // GDM_H_
