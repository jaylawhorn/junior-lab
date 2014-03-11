#ifndef MOSFXNS_HH
#define MOSFXNS_HH

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TString.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

void fillHist(const TString filename, // input file
	      TH1D* hist              // histogram to fill
	      );

void confParse(const TString conf,   // input conf file
	       const Int_t sel,      // filetype of interest
	       TH1D* hist // histogram to fill
               ) {
  
  ifstream ifs;
  ifs.open(conf.Data());
  assert(ifs.is_open());
  string line;
  TString fname;
  Int_t type;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    stringstream ss(line);
    ss >> fname >> type;

    if (type==sel) {
      cout << "fill " << fname << endl;
      fillHist(fname, hist);
    }
  }
  ifs.close();
}

void fillHist(const TString filename, // input file
	      TH1D* hist              // vector to store TH1Ds
	      ) {
  ifstream ifs;
  ifs.open(filename.Data());
  assert(ifs.is_open());
  string line;
  string a, b, c;
  Int_t channel;
  Double_t dwell;
  Int_t count;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;
    stringstream ss(line);
    getline(ss, a, ',');
    getline(ss, b, ',');
    getline(ss, c, ',');
    channel=atoi(a.c_str());
    dwell=atof(b.c_str());
    count=atof(c.c_str());
    for (Int_t i=0; i<count; i++) { hist->Fill(channel); }
  }
  ifs.close();

}

#endif
