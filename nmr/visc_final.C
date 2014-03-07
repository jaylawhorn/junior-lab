#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TGraphErrors.h>
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

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);
  c1->SetLogx();
  c1->SetLogy();

  TGaxis::SetMaxDigits(3);

  TGraphErrors *visc = new TGraphErrors();

  visc->SetPoint(0, 1410, 26.92);
  visc->SetPointError(0, 0, 7.99);

  visc->SetPoint(4, 22.5, 611.74);
  visc->SetPointError(4, 0, 1105.45);

  visc->SetPoint(1, 6.00, 78.05);
  visc->SetPointError(1, 0, 26.74);

  visc->SetPoint(2, 2.5, 73.31);
  visc->SetPointError(2, 0, 30.59);

  visc->SetPoint(3, 1.005, 2139);
  visc->SetPointError(3, 0, 321);

  visc->SetTitle();
  visc->GetXaxis()->SetTitle("Viscosity [centipoise]");
  visc->GetYaxis()->SetTitle("T_{1} [ms]");
  visc->Draw("ap");

  //TF1 *fitfxn = new TF1("fitfxn", "[0]/x+[1]", 0.5, 1500);
  //visc->Fit("fitfxn");

}
