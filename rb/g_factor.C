//----------------------------------------------------------------------------
//
// 
// 
// 
//
//----------------------------------------------------------------------------
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TGaxis.h>
#include <TMath.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TBox.h>
#include <TF1.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "plotStyle.hh"
#endif

void g_factor(TString file85="fin100_85.txt", TString file87="fin100_87.txt", Float_t freq=100) {

  vector<Float_t> m1;
  vector<Float_t> m2;
  vector<Float_t> m1err;
  vector<Float_t> m2err;

  ifstream ifs;
  ifs.open(file85.Data());
  assert(ifs.is_open());
  string line;
  Float_t temp=0, temp2=0;

  while (getline(ifs,line)) {
    stringstream ss(line);
    ss >> temp >> temp2;
    m1.push_back(temp);
    m1err.push_back(temp2);
  }
  ifs.close();

  ifs.open(file87.Data());
  assert(ifs.is_open());

  while (getline(ifs,line)) {
    stringstream ss(line);
    ss >> temp >> temp2;
    m2.push_back(temp);
    m2err.push_back(temp2);
  }
  ifs.close();

  Float_t w=0;
  Float_t avg1=0;
  Float_t dev1=0;

  for (Int_t i=0; i<m1.size(); i++) {
    w=1/(m1err[i]*m1err[i]);
    avg1+=m1[i]*w;
    dev1+=w;
  }

  avg1/=dev1;
  dev1=TMath::Sqrt(1/dev1);

  Float_t avg2=0;
  Float_t dev2=0;

  for (Int_t i=0; i<m2.size(); i++) {
    w=1/(m2err[i]*m2err[i]);
    avg2+=m2[i]*w;
    dev2+=w;
  }
  avg2/=dev2;
  dev2=TMath::Sqrt(1/dev2);

  cout << avg1 << " +- " << dev1 << endl;
  cout << avg2 << " +- " << dev2 << endl;
  cout << endl;

  Float_t bohr=9.1e-21;
  Float_t h=6.62e-27;
  
  Float_t cons=h*1e5/bohr;
  cout << cons/0.52 << endl;
  //cout << h*1e5/bohr << endl;

  cout << cons/avg1 << " +- " << cons/(avg1*avg1)*dev1 << endl; 
  //cout << (cons/avg1-1.0/3)*freq << " +- " << cons/(avg1*avg1)*dev1 << endl; 
  cout << cons/avg2 << " +- " << cons/(avg2*avg2)*dev2 << endl; 
  //cout << (cons/avg2-1.0/2)*freq << " +- " << cons/(avg2*avg2)*dev2 << endl; 
  cout << endl;

  cout << cons*((cons/avg1-1.0/3)*freq) << endl;
  cout << cons*((cons/avg2-1.0/2)*freq) << endl;

  cout << avg1/avg2 << " +- " << avg1/avg2*TMath::Sqrt(dev1*dev1/(avg1*avg1)+dev2*dev2/(avg2*avg2)) << endl;

  Float_t em1 = (4*TMath::Pi()*3e15)/(avg1/3);
  Float_t dem1 = em1/avg1*dev1; 
  Float_t em2 = (4*TMath::Pi()*3e15)/(avg2/2);
  Float_t dem2 = em2/avg2*dev2;

  Float_t em=em1/(dem1*dem1)+em2/(dem2*dem2);
  Float_t dem=1/(dem1*dem1)+1/(dem2*dem2);

  cout << em/dem << " +- " << TMath::Sqrt(1/dem) << endl;

}
