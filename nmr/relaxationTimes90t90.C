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
void relaxationTimes90t90() {

  const int n=20;

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  //******************************
  // from pg 12 of lab notebook
  //******************************

  Float_t times[n] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 18, 22, 26, 30, 46, 62, 78, 94, 110 };
  Float_t v1[n] = { 18.6, 19.8, 19.8, 21.0, 20.6, 22.2, 23.4, 23.0, 23.8, 24.0, 24.6, 25.6, 27.4, 30.2, 30.0, 31.6, 35.2, 36.4, 38.0, 38.0 };
  Float_t v2[n] = { 18.8, 19.6, 20.2, 20.4, 21.6, 22.0, 23.4, 23.4, 23.6, 24.6, 25.6, 25.8, 28.0, 28.0, 30.8, 34.0, 35.6, 36.0, 38.0, 38.4 };
  Float_t v3[n] = { 19.8, 19.8, 20.6, 20.8, 21.6, 22.0, 23.0, 23.0, 24.2, 24.2, 25.0, 26.0, 28.0, 28.6, 31.2, 34.0, 35.6, 36.0, 37.6, 38.8 };
  Float_t avg[n], std[n], err[n];

  for (Int_t i=0; i<n; i++) {
    avg[i] = (v1[i]+v2[i]+v3[i])/3.0;
    std[i] = (v1[i]*v1[i]+v2[i]*v2[i]+v3[i]*v3[i])/3.0;
    std[i] -= avg[i]*avg[i];
    std[i] = TMath::Sqrt(std[i]);
    err[i] = TMath::Sqrt(std[i]*std[i]+1.1*1.1);
    cout << avg[i] << ", " << err[i] << endl;
  }

  TGraphErrors *grT1 = new TGraphErrors(n, times, avg, 0, err);

  TF1 *t1fit = new TF1("t1fit", "[0]-expo(1)", 0, 115);

  grT1->SetTitle("90-tau-90");
  grT1->GetXaxis()->SetTitle("tau [ms]");
  grT1->GetYaxis()->SetTitle("Peak Value [mV]");
  grT1->Fit("t1fit", "R");

  cout << endl;
  cout << "T1 = " << -1.0/t1fit->GetParameter(2) << endl;

  grT1->Draw("ap");

  c1->SaveAs("t1_90t90.png");

}
