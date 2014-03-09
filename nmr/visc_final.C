#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TBox.h>
#include <TF1.h>
#include <TProfile.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "plotstyle.hh"
#endif

void visc_final() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 600, 600);
  c1->SetLogx();
  c1->SetLogy();

  TGaxis::SetMaxDigits(3);

  TGraphAsymmErrors *visc = new TGraphAsymmErrors();

  // pure glycerine
  visc->SetPoint(0, 1410.0/100, 27.14/1000);
  visc->SetPointError(0, 0, 0, 0.96/1000, 0.96/1000);

  // 70%
  visc->SetPoint(1, 22.5/100, 125.3/1000);
  visc->SetPointError(1, 0, 0, 125.3/1000, 474.0/1000);

  // 50%
  visc->SetPoint(2, 6.00/100, 35.3/1000);
  visc->SetPointError(2, 0, 0, 10.0/1000, 110.0/1000);

  // 30%
  visc->SetPoint(3, 2.5/100, 112.9/1000);
  visc->SetPointError(3, 0, 0, 80./1000, 348./1000);

  visc->SetPoint(4, 1.005/100,2100./1000);
  visc->SetPointError(4, 0, 0, 1300./1000, 1300./1000);

  visc->SetTitle();
  visc->GetXaxis()->SetTitle("Viscosity [poise]");
  visc->GetYaxis()->SetTitle("T_{1} [s]");

  visc->GetYaxis()->SetRangeUser(0.01, 10);
  visc->Draw("ap");

  c1->SaveAs("visc_final.png");

  //TF1 *fitfxn = new TF1("fitfxn", "[0]/x+[1]", 0.5, 1500);
  //visc->Fit("fitfxn");

}
