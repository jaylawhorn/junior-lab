#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
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

#endif
void testFit(const TString filename="data/Viscosity_measurements_2/3t15_2.csv") {

  TGraph *gr = new TGraph(filename, "%lg %lg", ",");

  TH1D *hist = new TH1D("hist", "hist", 100, -2, 0);

  Double_t x, y;

  Double_t mean=0, stdev=0;

  for (Int_t i=0; i<1000; i++) {
    gr->GetPoint(i, x, y);
    hist->Fill(y);
    mean+=y;
    stdev+=y*y;
  }

  mean/=1000;
  stdev/=1000;
  stdev-=mean*mean;
  stdev=TMath::Sqrt(stdev);

}
