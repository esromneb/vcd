#include "playback.hpp"

#include <iostream>
#include <string>


using std::cout;
using std::endl;


int main(void) {
  Snapshot snap;

  // std::map<int,std::string> map;
  // map.emplace(4,"d");

  // snap.map.insert(std::string("a"),std::string("b"));
  
  snap.map.emplace("a","b");
  snap.map.emplace("c","d");

  // snap.map.insert(std::pair("y","y"));

  // snap.map.put(std::pair("y","z"));


  snap.map["a"] = "z";

  // snap.map.put("a","z");

  // for(auto p : snap.map ) {
  //   auto [k,v] = p;
  //   cout << k << " - " << v << "\n";
  // }

  Snapshot s2 = snap;


  s2.map["foo"] = "bar";

  for(auto p : s2.map ) {
    auto [k,v] = p;
    cout << k << " - " << v << "\n";
  }

  cout << "\n";

  for(auto p : snap.map ) {
    auto [k,v] = p;
    cout << k << " - " << v << "\n";
  }


}

