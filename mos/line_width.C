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

void line_width(Int_t i=7) {

  TH1D *fecn = new TH1D("fecn", "fecn", 2048, 0, 2048);

  confParse("data_list.txt", i, fecn);
  fecn->Draw("same");

  TF1 *fitPeak = new TF1("fitPeak", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*(x-[5])^2", 200, 1800);

  fitPeak->SetParameter(0, 5600);
  fitPeak->SetParameter(1, 20);
  fitPeak->SetParameter(2, 1050);
  fitPeak->SetParameter(3, 15);

  fitPeak->SetLineColor(kBlue);
  fecn->Fit("fitPeak", "RN");
  fitPeak->Draw("same");

  cout << fitPeak->GetParameter(3) << endl;

}
