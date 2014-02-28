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
void carrPurcell(const TString filename="data/Carr-Purcell_glycerine/cp4_2.csv") {

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  TGraph *gr_sig = new TGraph(filename, "%lg %lg", ",");
  TGraph *gr_pulse = new TGraph(filename, "%lg %*lg %lg", ",");
  TGraph *gr_sel = new TGraph();

  gr_sel->SetMarkerColor(kBlue);
  gr_sig->Draw("al");
  gr_pulse->SetLineColor(kRed);
  gr_pulse->Draw("same");

  Double_t x, y;
  Double_t temp, pulse;

  Bool_t start=kFALSE;
  Bool_t count=kTRUE;
  Int_t tt=0;

  Double_t max_x=-1, max_y=-1;
  Int_t j=0;

  for (Int_t i=2; i<gr_sig->GetN(); i++) {

    gr_pulse->GetPoint(i,temp,pulse);

    if (pulse>1.0) {
      tt=0;
      if (max_y!=-1) { gr_sel->SetPoint(j,max_x,max_y); j++; }
      max_x=-1; max_y=-1;
      count=kFALSE;
      //if (start==kFALSE) cout << "first pulse!" << endl;
      start=kTRUE;
    }
    else if (start==kFALSE) continue;
    else if (temp<0.0025) continue;
    else if ((count==kFALSE) && (tt<15)) {
      tt++;
    }
    else if (count==kFALSE) {
      count=kTRUE;
      tt=0;
    }
    else if (count==kTRUE) {
      gr_sig->GetPoint(i,x,y);
      if (y>max_y) {
	max_y=y;
	max_x=x;
      }
    }      
  }

  gr_sel->Draw("same p");

  TF1 *fitfxn = new TF1("fitfxn","[0]+[1]*exp([2]*x)", 0.0, 0.07);
  gr_sel->Fit("fitfxn");

  cout << endl;
  cout << -1000/fitfxn->GetParameter(2) << endl;

  c1->SaveAs(filename(28,5)+TString(".png"));

}
