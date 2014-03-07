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
void relaxationTimes180t90() {

  const int n=8;

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  //******************************
  // from pg 13 of lab notebook
  //******************************

  Float_t times[n] = { 4, 12, 28, 44, 60, 16, 20, 24 };
  Float_t v1[n] = { 15.8, 11.2, 23.2, 30.4, 34.0, 14.8, 17.6, 20.4 };
  Float_t v2[n] = { 16.2, 10.4, 23.2, 30.8, 34.4, 14.6, 18.0, 20.6 };
  Float_t v3[n] = { 16.0, 10.4, 23.8, 31.2, 34.0, 14.4, 17.4, 20.8 };
  Float_t delay[n] = { 500, 384, 308, 320, 304, 312, 320, 312 };
  Float_t avg[n], std[n], err[n];

  for (Int_t i=0; i<n; i++) {
    delay[i]/=1000.0;
    times[i]+=delay[i];
    avg[i] = (v1[i]+v2[i]+v3[i])/3.0;
    std[i] = (v1[i]*v1[i]+v2[i]*v2[i]+v3[i]*v3[i])/3.0;
    std[i] -= avg[i]*avg[i];
    std[i] = TMath::Sqrt(std[i]);
    err[i] = TMath::Sqrt(std[i]*std[i]/3+1.1*1.1);
    cout << times[i] << ", " << avg[i] << ", " << err[i] << endl;
  }

  TGraphErrors *grT1 = new TGraphErrors(n, times, avg, 0, err);

  TF1 *t1fit = new TF1("t1fit", "[0]-expo(1)", 10, 75);
  t1fit->SetLineColor(kBlue);
  
  grT1->Fit("t1fit", "R");

  cout << endl;

  Float_t t1 = -1.0/t1fit->GetParameter(2);
  Double_t uncert = max(fabs(t1+(1.0/(t1fit->GetParameter(2)+t1fit->GetParError(2)))), fabs(t1+(1.0/(t1fit->GetParameter(2)-t1fit->GetParError(2)))));

  cout << fabs(t1+(1.0/(t1fit->GetParameter(2)+t1fit->GetParError(2)))) << ", " << fabs(t1+(1.0/(t1fit->GetParameter(2)-t1fit->GetParError(2)))) << endl;
  cout << "T1 = " << -1.0/t1fit->GetParameter(2) << " +- " << uncert << endl; 

  grT1->SetTitle("");
  grT1->GetXaxis()->SetTitle("Tau [ms]");
  grT1->GetYaxis()->SetTitle("Max Voltage [mV]");
  grT1->Draw("ap");

  c1->SaveAs("images/t1_180t90.png");

}
