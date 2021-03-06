//----------------------------------------------------------------------------
//
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

#include "mosfxns.hh"
#include "plotStyle.hh"
#endif

void raw_spec() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);
  //TGaxis::SetMaxDigits(3);

  TH1D *spec1 = new TH1D("spec1", "spec1", 256, 0, 2048);
  TH1D *spec2 = new TH1D("spec2", "spec2", 256, 0, 2048);

  confParse("data_list.txt", 99, spec1);
  confParse("data_list.txt", 999, spec2);

  Double_t scale = spec1->GetEntries();
  spec1->Scale(1.0/scale);
  scale=spec2->GetEntries();
  spec2->Scale(1.0/scale);

  spec1->SetTitle("");
  spec1->GetXaxis()->SetTitle("MCA Bin");
  spec1->GetXaxis()->SetNdivisions(8,5,0);
  spec1->GetYaxis()->SetTitle("A.U.");

  spec1->Draw("");
  spec2->SetLineColor(kRed);
  spec2->Draw("same");

  c1->SaveAs("raw_spec.png");

}
