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

#include "confparse.hh"

#endif
Double_t getMax(const TString filename, Float_t tau, Int_t n, TString dir);

void viscGraph(TString conf, TString dir, TGraph *viscgr);

void viscosity_t1() {

  TGraph *g30 = new TGraph(9);
  TGraph *g50 = new TGraph(9);
  TGraph *g70 = new TGraph(9);

  viscGraph("g30_180t90.txt", "g30", g30);
  viscGraph("g50_180t90.txt", "g50", g50);
  viscGraph("g70_180t90.txt", "g70", g70);
  
  g30->SetMarkerColor(kBlue);
  g50->SetMarkerColor(kRed);

  g70->Draw("ap");
  g70->GetYaxis()->SetRangeUser(-0.04,0.04);
  g50->Draw("same p");
  g30->Draw("same p");
  
}

void viscGraph(TString conf, TString dir, TGraph *viscgr) {

  vector<TString> fnamev;
  vector<Int_t> timev;
  vector<Double_t> maxv;

  confParse(conf, fnamev, timev);

  for (Int_t i=0; i<fnamev.size(); i++) {

    maxv.push_back(getMax(fnamev[i], timev[i], i, dir));
    cout << timev[i] << " " << maxv[i] << endl;

  }

  Double_t temp=0;

  for (Int_t i=0; i<9; i++) {

    temp=-(maxv[3*i]+maxv[3*i+1]+maxv[3*i+2])/3;
    viscgr->SetPoint(i,timev[3*i], temp);

    cout << timev[3*i] << " " << timev[3*i+1] << " " << timev[3*i+2] << " " << temp << endl;
    temp=0;

  }

  Double_t x, y;
  
  Double_t ymean=viscgr->GetMean(2);

  for (Int_t i=0; i<9; i++) {
    
    viscgr->GetPoint(i, x, y);

    viscgr->SetPoint(i, x, y-ymean);

  }

}

Double_t getMax(const TString filename, Float_t tau, Int_t n, TString dir) {

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  TGraph *gr_sig = new TGraph(filename, "%lg %lg", ",");
  TGraph *gr_pulse = new TGraph(filename, "%lg %*lg %lg", ",");
  TGraph *gr_sel = new TGraph();

  gr_sel->SetMarkerColor(kBlue);
  gr_sig->Draw("al");

  Double_t x, y;
  Double_t temp, pulse;

  Bool_t start=kFALSE;
  Bool_t count=kTRUE;
  Int_t tt=0;

  Double_t min_x=-1, min_y=-1;
  Double_t max_x=-1, max_y=-1;
  Int_t j=0;

  for (Int_t i=2; i<gr_sig->GetN(); i++) {

    gr_pulse->GetPoint(i,temp,pulse);

    if (pulse>1.0) {
      tt=0;
      max_x=-1; max_y=-1;
      min_x=1; min_y=1;
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
      if (y<min_y) {
	min_y=y;
	min_x=x;
      }
      if (y>max_y) {
	max_y=y;
	max_x=x;
      }
    }      
  }

  //cout << tau << " " << max_x << " " << max_y << endl;

  gr_sel->SetPoint(j,max_x, max_y);

  gr_sel->Draw("same p");

  char outfile[50];

  sprintf(outfile, "%s/t%.0f_%i.png",dir.Data(),tau,n);

  c1->SaveAs(outfile);

  delete c1;

  Float_t avg=0.5*(max_y+min_y);
  cout << max_y << " " << min_y << " " << avg << endl;

  return max_y-avg;

}
