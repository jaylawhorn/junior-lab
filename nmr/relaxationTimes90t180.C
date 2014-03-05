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
void relaxationTimes90t180() {

  const int n=16;

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  //******************************
  // from pg 9 of lab notebook
  //******************************

  Float_t times[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 16, 19, 23, 26, 29};
  Float_t values[n] = {65.2, 58.4, 52.4, 47.2, 42.0, 37.6, 33.2, 29.6, 26.8, 23.2, 17.2, 12.4, 8.88, 6.5, 4.9, 4.0};
  Float_t uncert[n] = {0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 0.8, 1.4, 1.4, 0.4, 1.0, 1.5, 0.4};

  TGraphErrors *grT2 = new TGraphErrors(n, times, values, 0, uncert);

  TF1 *t2fit = new TF1("t2fit", "expo", 0, 30);

  grT2->Fit("t2fit");

  cout << endl;
  cout << "T2 = " << -1.0/t2fit->GetParameter(1) << endl;

  grT2->SetTitle("90-tau-180");
  grT2->GetXaxis()->SetTitle("Tau [ms]");
  grT2->GetYaxis()->SetTitle("Peak Value [mV]");
  grT2->Draw("ap");

  c1->SaveAs("images/t2_90t180.png");

}
