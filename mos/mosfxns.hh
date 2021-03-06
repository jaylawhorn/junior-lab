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

void getVel(vector<Double_t> peak, vector<Double_t> peakunc, TF1 *velocityCurve) {

  vector<Double_t> vel;
  vector<Double_t> velunc;

  if (peak.size()==6) {
    vel.push_back(-6.18); velunc.push_back(0.17);
    vel.push_back(-3.63); velunc.push_back(0.16);
    vel.push_back(-1.10); velunc.push_back(0.11);
    vel.push_back(0.83); velunc.push_back(0.08);
    vel.push_back(3.37); velunc.push_back(0.08);
    vel.push_back(5.92); velunc.push_back(0.11);
  }
  else if (peak.size()==4) {
    vel.push_back(-3.63); velunc.push_back(0.16);
    vel.push_back(-1.10); velunc.push_back(0.11);
    vel.push_back(0.83); velunc.push_back(0.08);
    vel.push_back(3.37); velunc.push_back(0.08);
  }
  else {
    cout << "Not enough peaks!" << endl;
  }

  TGraphErrors *velGraph = new TGraphErrors(peak.size(), &(peak[0]), &(vel[0]), &(peakunc[0]), &(velunc[0]));

  velGraph->Draw("apl");

  velGraph->Fit(velocityCurve->GetName(), "EMNR");

}

Double_t getVelUnc(Double_t peak) {

  vector<Double_t> vel;
  vector<Double_t> velunc;

  vel.push_back(-6.18); velunc.push_back(0.17);
  vel.push_back(-3.63); velunc.push_back(0.16);
  vel.push_back(-1.10); velunc.push_back(0.11);
  vel.push_back(0.83); velunc.push_back(0.08);
  vel.push_back(3.37); velunc.push_back(0.08);
  vel.push_back(5.92); velunc.push_back(0.11);

  TGraph *vvsu = new TGraph(6, &vel[0], &velunc[0]);
  TF1 *gunc = new TF1("gunc", "[0]+[2]*x^2", -10, 10);

  vvsu->Fit("gunc");

  return gunc->Eval(peak);

}

#endif
