#include "playback.hpp"

#include <vector>
#include <iostream>
#include <tuple>



#include <iostream>


using std::cout;
// using std::cin;
using std::endl;

static std::string strForId(const uint8_t id) {
  switch(id) {
    case ID_COMMAND:
      return "ID_COMMAND";
      break;
    case ID_SCOPEID:
      return "ID_SCOPEID";
      break;
    case ID_VARSIZE:
      return "ID_VARSIZE";
      break;
    case ID_VARID:
      return "ID_VARID";
      break;
    case ID_VARNAME:
      return "ID_VARNAME";
      break;
    case ID_IDSPAN:
      return "ID_IDSPAN";
      break;
    case ID_VECTORSPAN:
      return "ID_VECTORSPAN";
      break;
    case ID_TIMESPAN:
      return "ID_TIMESPAN";
      break;
    default:
      return "?";
      break;
  }
}

static std::string strForCommand(const uint8_t command) {
  switch(command) {
    case 14:
      return "0";
      break;
    case 15:
      return "1";
      break;
    default:
      return "?";
      break;
  }
}

// calls exit when a bad value is passed
static void enforceValue(const uint8_t id) {
    switch(id) {
    case ID_COMMAND:
      // cout << "ID_COMMAND\n";
      break;
    case ID_SCOPEID:
      // cout << "ID_SCOPEID\n";
      break;
    case ID_VARSIZE:
      // cout << "ID_VARSIZE\n";
      break;
    case ID_VARID:
      // cout << "ID_VARID\n";
      break;
    case ID_VARNAME:
      // cout << "ID_VARNAME\n";
      break;
    case ID_IDSPAN:
      // cout << "ID_IDSPAN\n";
      break;
    case ID_VECTORSPAN:
      // cout << "ID_VECTORSPAN\n";
      break;
    case ID_TIMESPAN:
      // cout << "ID_TIMESPAN\n";
      break;
    default:
      // cout << "Illegal id argument";
      exit(1);
      break;
  }
}

void Snapshot::span(const vcd_parser_t* const state, const unsigned char* const p, const unsigned char* const endp) {
  std::vector<char> lcl;
  lcl.assign(p,endp);

  std::string lcls = std::string(lcl.begin(), lcl.end());

  // map[state->cmd]
  map[lcls] = state->command;
  // cout << "id " << " with " << (int)state->command << "\n";
}

void Snapshot::streamIn(const uint8_t id, const vcd_parser_t* const state, const unsigned char* const p, const unsigned char* const endp) {
  switch(id) {
    case ID_COMMAND:
      break;
    case ID_SCOPEID:
      break;
    case ID_VARSIZE:
      break;
    case ID_VARID:
      break;
    case ID_VARNAME:
      break;
    case ID_IDSPAN:
      span(state, p, endp);
      break;
    case ID_VECTORSPAN:
      break;
    case ID_TIMESPAN:
      time = strtol((const char *)p, (char **)&endp, 10);
      break;
    default:
      exit(1);
      break;
  }
}

std::string Snapshot::asString(void) {
  std::string out;

  for(auto p : map ) {
    auto [k,v] = p;

    if( k[k.size()-1] == 10 ) {
      std::string k2 = k.substr(0, k.size()-1);
      out += k2;
    } else {
      out += k;
    }

    out += " -> ";
    out += strForCommand(v[0]);
    out += "\n";
  }

  return out;
}



Playback::Playback() {
  cout << "ctons()\n";
}


void Playback::cppEntry(
  const uint8_t id,
  const vcd_parser_t* const state,
  const unsigned char* const p,
  const unsigned char* const endp) {
    // cout << "In callback\n";

  enforceValue(id);
  

  if( id == ID_TIMESPAN ) {
    started = true;
  }

  // for now, skip header section, will address this later
  if( !started ) {
    return;
  }

  std::vector<char> lcl;
  lcl.assign(p,endp);
  dense.push_back(std::make_tuple(id,state->command,lcl));

  snap.streamIn(id, state, p, endp);


  switch(id) {
    case ID_COMMAND:
      break;
    case ID_SCOPEID:
      break;
    case ID_VARSIZE:
      break;
    case ID_VARID:
      break;
    case ID_VARNAME:
      break;
    case ID_IDSPAN:
      break;
    case ID_VECTORSPAN:
      break;
    case ID_TIMESPAN:
      snaps.push_back(snap);
      break;
    default:
      break;
  }


}

// void Playback::timespan(const uint64_t t) {

// }

void Playback::debug1() {
  cout << "In debug1()" << "\n";

  cout << "saved " << snaps.size() << " snapshots\n";

  for(int i = 0; i < 10; i++) {
    cout << "snap: " << i << "\n";

    cout << snaps[i].asString();
  }


}
void Playback::debug0() {
  debug1(); return;

  cout << "In debug0()" << "\n";

  size_t endout = dense.size();

  for(size_t i = 0; i < endout; i++) {
    auto [id,cmd,one] = dense[i];
    // cout << "\nStart " << strForId(id) << " " << i << "\n";
    cout << "\nStart " << strForId(id) << " (" << strForCommand(cmd) << ")" << "\n";
    for(size_t j = 0; j < one.size(); j++) {
      cout << one[j];
    }
    cout << "\n";
    for(size_t j = 0; j < one.size(); j++) {
      cout << (int)one[j] << " " ;
    }
    cout << "\n";
  }

}