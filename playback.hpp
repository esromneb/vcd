#pragma once

#include "vcd_types.h"

#include <vector>
#include <tuple>
#include <cstdint>



class Playback {
public:
  
  bool started = false;
  std::vector<std::tuple<uint8_t,std::vector<char>>> vv;

  uint64_t now = -1; // now for the parser


  Playback();
  void cppEntry(const uint8_t id, const unsigned char* const p, const unsigned char* const endp);
  void debug0();
  void timespan(const uint64_t t);
};