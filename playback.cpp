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




Playback::Playback() {
  cout << "ctons()\n";
}


void Playback::cppEntry(
  const uint8_t id,
  const unsigned char* const p,
  const unsigned char* const endp) {
    // cout << "In callback\n";

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

  if( id == ID_TIMESPAN ) {
    started = true;
  }

  if( !started ) {
    return;
  }

  std::vector<char> lcl;
  lcl.assign(p,endp);
  vv.push_back(std::make_tuple(id,lcl));
}

void Playback::timespan(const uint64_t t) {
  
}

void Playback::debug0() {
  cout << "In debug0()" << "\n";

  size_t endout = vv.size();

  for(size_t i = 0; i < endout; i++) {
    auto [id,one] = vv[i];
    // cout << "\nStart " << strForId(id) << " " << i << "\n";
    cout << "\nStart " << strForId(id) << "\n";
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