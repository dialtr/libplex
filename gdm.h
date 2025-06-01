// Copyright (C) 2025 Thomas R. Dial
#ifndef GDM_H_
#define GDM_H_

namespace plex {

class GDM {
 public:
  enum class ScanType {
    kNone = 0,    // Undefined scan type.
    kServer = 1,  // Scan for servers.
    kClient = 2   // Scan for clients.
  };

  // Create new instance.
  static GDM* New(GDM::ScanType type);

  // Destroy the instance.
  ~GDM();

 private:
  GDM(int fd, GDM::ScanType type, const char* ip, unsigned short port);
  int fd_ = -1;
  GDM::ScanType scan_type_ = ScanType::kNone;
  const char* ip_address_ = nullptr;
  unsigned short port_ = 0;
};

}  // namespace plex

#endif  // GDM_H_
