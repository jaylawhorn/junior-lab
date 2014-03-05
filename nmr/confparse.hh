#ifndef CONFPARSE_HH
#define CONFPARSE_HH

#include <vector>                   // STL vector class
#include <fstream>                  // functions for file I/O
#include <string>                   // C++ string class
#include <sstream>                  // class for parsing strings
#include <TString.h>                // ROOT string class

void confParse(const TString    conf,      // input conf file
               vector<TString>  &fnamev,   // vector to store sample files
	       vector<Int_t>    &timev     // vector to store sample times
	       ) {

  ifstream ifs;
  ifs.open(conf.Data());
  assert(ifs.is_open());
  string line;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    
    string fname;
    Int_t time;
    stringstream ss(line);
    ss >> fname >> time;

    fnamev.push_back(fname);
    timev.push_back(time);
    
  }
  ifs.close();
}
#endif
