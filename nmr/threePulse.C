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
#include <TProfile.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

#endif
void getMax(const TString filename, Float_t tau, Int_t n, TProfile *final);

void threePulse() {

  const Int_t n=33;

  TProfile *final = new TProfile("final", "final", 65, 0, 65);

  Float_t tau[n] = {12, 24, 3, 60, 12, 24, 3, 60, 12, 24, 3, 60, 16, 28, 44, 8, 16, 28, 44, 8, 16, 28, 44, 8, 20, 36, 52, 20, 36, 52, 20, 36, 52 };

  TString files[n] = { "data/Three-pulse measurements/t12_0.csv", "data/Three-pulse measurements/t24_0.csv", "data/Three-pulse measurements/t3_0.csv", "data/Three-pulse measurements/t60_0.csv",
		       "data/Three-pulse measurements/t12_1.csv", "data/Three-pulse measurements/t24_1.csv", "data/Three-pulse measurements/t3_1.csv", "data/Three-pulse measurements/t60_1.csv",
		       "data/Three-pulse measurements/t12_2.csv", "data/Three-pulse measurements/t24_2.csv", "data/Three-pulse measurements/t3_2.csv", "data/Three-pulse measurements/t60_2.csv",
		       "data/Three-pulse measurements/t16_0.csv", "data/Three-pulse measurements/t28_0.csv", "data/Three-pulse measurements/t44_0.csv", "data/Three-pulse measurements/t8_0.csv",
		       "data/Three-pulse measurements/t16_1.csv", "data/Three-pulse measurements/t28_1.csv", "data/Three-pulse measurements/t44_1.csv", "data/Three-pulse measurements/t8_1.csv",
		       "data/Three-pulse measurements/t16_2.csv", "data/Three-pulse measurements/t28_2.csv", "data/Three-pulse measurements/t44_2.csv", "data/Three-pulse measurements/t8_2.csv",
		       "data/Three-pulse measurements/t20_0.csv", "data/Three-pulse measurements/t36_0.csv", "data/Three-pulse measurements/t52_0.csv",
		       "data/Three-pulse measurements/t20_1.csv", "data/Three-pulse measurements/t36_1.csv", "data/Three-pulse measurements/t52_1.csv",
		       "data/Three-pulse measurements/t20_2.csv", "data/Three-pulse measurements/t36_2.csv", "data/Three-pulse measurements/t52_2.csv" };
  
  for (Int_t i=0; i<n; i++) {

    getMax(files[i], tau[i], i, final);

  }

  TGraph *moreFinal = new TGraph(11);

  Int_t j=0;
  for (Int_t i=0; i<final->GetNbinsX(); i++) {

    if (final->GetBinContent(i) > 0) {
      moreFinal->SetPoint(j, final->GetBinCenter(i), final->GetBinContent(i));
      j++;
    }

  }

  TCanvas *c = new TCanvas("c", "c", 800, 600);

  TF1 *t1fit = new TF1("t1fit", "[0]-expo(1)", 15, 65);

  moreFinal->Fit("t1fit", "R");

  cout << endl;
  cout << "T1 = " << -1.0/t1fit->GetParameter(2) << endl;

  moreFinal->SetTitle("Three pulse");
  moreFinal->GetXaxis()->SetTitle("Tau [ms]");
  moreFinal->GetYaxis()->SetTitle("Max Voltage [mV]");
  moreFinal->Draw();

  c->SaveAs("threePulse.png");  

}

void getMax(const TString filename, Float_t tau, Int_t n, TProfile *final) {

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
      max_x=-1; max_y=-1;
      count=kFALSE;
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

  //cout << filename(30,4) << endl;
  cout << tau << " " << max_x << " " << max_y << endl;

  gr_sel->SetPoint(j,max_x, max_y);

  gr_sel->Draw("same p");

  final->Fill(tau, max_y);

  char outfile[50];

  sprintf(outfile, "images/t%.0f_%i.png",tau,n);

  c1->SaveAs(outfile);

  delete c1;

}
