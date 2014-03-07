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

void water_final() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 600, 600);

  TGaxis::SetMaxDigits(3);

  TGraphErrors *o2 = new TGraphErrors("test_water_o2.txt");
  TGraphErrors *n2 = new TGraphErrors("test_water_n2.txt");

  n2->SetMarkerColor(kBlue);
  o2->SetMarkerColor(kRed);

  n2->SetTitle("");
  n2->GetXaxis()->SetTitle("Repeat Time [s]");
  n2->GetYaxis()->SetTitle("Spin Echo Voltage [mV]");

  TLegend *leg = new TLegend(0.3, 0.5, 0.5, 0.7);
  leg->AddEntry(n2, "N_{2} bubbled", "p");
  leg->AddEntry(o2, "O_{2} dissolved", "p");
  n2->Draw("ap");
  leg->Draw();
  o2->Draw("same p");

}
