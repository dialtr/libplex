// Copyright (C) 2025 Thomas R. Dial
#include <assert.h>
#include <errno.h>

#include <iostream>
#include <memory>

#include "gdm.h"

using ::plex::GDM;
using ::std::cerr;
using ::std::cout;
using ::std::endl;

int main(int argc, char* argv[]) {
  // Create a new GDM object to scan for servers.
  GDM* gdm = GDM::New(GDM::ScanType::kServer);

  // Make sure we got it.
  assert(gdm != nullptr);
  if (gdm) {
    cout << "Created GDM object." << endl;
  } else {
    cerr << "Error " << errno << " attempting to create GDM object." << endl;
    return 1;
  }

  // Delete the GDM object.
  delete gdm;
  cout << "Destroyed GDM object." << endl;

  return 0;
}
