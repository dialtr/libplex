// Copyright (C) 2025 Thomas R. Dial
#ifndef GDM_H_
#define GDM_H_

namespace plex {

class GDM {
 public:
  enum class ScanType {
    kServer = 0,  // Scan for servers.
    kClient = 1   // Scan for clients.
  };

  // Create new instance.
  static GDM* New(GDM::ScanType type);

  // Destroy the instance.
  ~GDM();

 private:
  GDM(int fd, GDM::ScanType type);
};

}  // namespace plex

#endif  // GDM_H_
