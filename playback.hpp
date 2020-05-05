#pragma once

#include "vcd_types.h"
#include "vcd_parser.h"

#include <vector>
#include <tuple>
#include <cstdint>
#include <string>
#include <map>


class Snapshot {
public:
  uint64_t time = 0;
  std::map<std::string,std::string> map;
  void streamIn(const uint8_t id, const vcd_parser_t* const state, const unsigned char* const p, const unsigned char* const endp);
};


class Playback {
public:
  
  bool started = false;
  std::vector<std::tuple<uint8_t,std::vector<char>>> dense;

  // uint64_t time = -1; // now for the parser

  Snapshot snap;


  Playback();
  void cppEntry(const uint8_t id, const vcd_parser_t* const state, const unsigned char* const p, const unsigned char* const endp);
  void debug0();
  // void timespan(const uint64_t t);
};
