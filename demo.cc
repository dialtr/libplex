// Copyright (C) 2025 Thomas R. Dial
#include <assert.h>

#include <memory>

#include "gdm.h"

using ::plex::GDM;

int main(int argc, char* argv[]) {
  // Create a new GDM object to scan for servers.
  GDM* gdm = GDM::New(GDM::ScanType::kServer);

  // Make sure we got it.
  assert(gdm != nullptr);

  // Delete the GDM object.
  delete gdm;

  return 0;
}
