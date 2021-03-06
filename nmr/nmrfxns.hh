#ifndef NMRFXNS_HH
#define NMRFXNS_HH

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
#include <TF1.h>
#include <TProfile.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

Double_t getUncert(const TString filename);

void getPeaks(const TString filename, Float_t tau, Int_t n, TString dir, TGraphErrors *gr_sel);

void confParse(const TString    conf,      // input conf file
               vector<TString>  &fnamev,   // vector to store sample files
               vector<Double_t> &timev,    // vector to store sample times
	       vector<TGraphErrors*>  &graphv    // fector to store TGraphs
               ) {

  ifstream ifs;
  ifs.open(conf.Data());
  assert(ifs.is_open());
  string line;
  while(getline(ifs,line)) {
    if(line[0]=='#') continue;

    string fname;
    Double_t time;
    stringstream ss(line);
    ss >> fname >> time;

    fnamev.push_back(fname);
    timev.push_back(time);
    graphv.push_back(new TGraphErrors());

  }
  ifs.close();
}

void getPeaks(const TString filename, Float_t tau, Int_t n, TString dir, TGraphErrors *gr_sel) {

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

  TGraph *gr_pulse = new TGraph(filename, "%lg %*lg %lg", ",");
  TGraph *gr_sig = new TGraph(filename, "%lg %lg", ",");

  gr_sel->SetMarkerColor(kBlue);
  gr_sig->Draw("al");

  Double_t x, y;
  Double_t xpulse, ypulse;
  Double_t temp;

  Bool_t start=kFALSE;

  Double_t min_x=1, min_y=1;
  Double_t max_x=-1, max_y=-1;

  Double_t dx=getUncert(filename);

  Int_t j=0;

  Double_t y_avg = gr_sig->GetMean(2);

  for (Int_t i=0; i<gr_sig->GetN(); i++) {

    gr_sig->GetPoint(i,x,y);
    gr_sig->SetPoint(i, 1000*x, 1000*y);
    gr_pulse->GetPoint(i, xpulse, ypulse);

    if (ypulse>1.0) {
      start=kTRUE;
      continue;
    }

    if (start!=kTRUE) continue;

    if ((y>y_avg) && (y>max_y)) {
      max_y=y;
      max_x=x;
    }
    if ((y<y_avg) && (y<min_y)) {
      min_y=y;
      min_x=x;
    }
    if ((y<y_avg) && (max_y!=-1)) {
      gr_sel->SetPoint(j, max_x, max_y); 
      gr_sel->SetPointError(j,0,dx);
      j++; max_x=-1; max_y=-1;
    }
    if ((y>y_avg) && (min_y!=1)) {
      gr_sel->SetPoint(j, min_x, min_y);
      gr_sel->SetPointError(j,0,dx);
      j++; min_x=1; min_y=1;

    }

  }

  Double_t start_draw=0, end_draw=0;
  gr_sel->GetPoint(1, start_draw, temp);
  gr_sel->GetPoint(j-1, end_draw, temp);

  gr_sig->SetLineWidth(2);
  gr_sig->SetTitle();
  gr_sig->GetXaxis()->SetTitle("Time [ms]");
  gr_sig->GetYaxis()->SetTitle("Voltage [mV]");
  
  gr_sig->GetXaxis()->SetRangeUser((start_draw-0.001)*1000, (end_draw+0.001)*1000);

  char outfile[50];

  sprintf(outfile, "%s/t%.1f_%i.png",dir.Data(),tau,n);

  c1->SaveAs(outfile);

  delete c1;

}

Double_t getUncert(const TString filename) {

  TGraph *gr = new TGraph(filename, "%lg %lg", ",");
  
  TH1D *hist = new TH1D("hist", "hist", 100, -2, 0);
  
  Double_t x, y;
  
  Double_t mean=0, stdev=0;
  
  for (Int_t i=0; i<1000; i++) {
    gr->GetPoint(i, x, y);
    hist->Fill(y);
    mean+=y;
    stdev+=y*y;
  }
  
  mean/=1000;
  stdev/=1000;
  stdev-=mean*mean;
  stdev=TMath::Sqrt(stdev);

  return stdev;
 
}

void carrPurcell(const TString filename, Int_t n, TString dir, TGraphErrors *gr_sel) {
   
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  TGraph *gr_pulse = new TGraph(filename, "%lg %*lg %lg", ",");
  TGraph *gr_sig = new TGraph(filename, "%lg %lg", ",");

  gr_sel->SetMarkerColor(kBlue);
  gr_sig->Draw("al");

  Double_t x, y;
  Double_t xpulse, ypulse;
  Double_t temp;

  Bool_t start=kFALSE;

  Double_t min_x=1, min_y=1;
  Double_t max_x=-1, max_y=-1;

  Double_t dx=0.4e-3;

  Int_t j=0;

  Double_t y_avg = gr_sig->GetMean(2);

  for (Int_t i=0; i<gr_sig->GetN(); i++) {

    gr_sig->GetPoint(i,x,y);
    gr_pulse->GetPoint(i, xpulse, ypulse);

    if (x>-0.005) {
      start=kTRUE;
    }
    if (x>0.07) { 
      start=kFALSE; 
    }

    if (start!=kTRUE) continue;

    if ((y>y_avg) && (y>max_y)) {
      max_y=y;
      max_x=x;
    }
    if ((y<y_avg) && (y<min_y)) {
      min_y=y;
      min_x=x;
    }
    if ((y<y_avg) && (max_y!=-1)) {
      gr_sel->SetPoint(j, max_x, max_y); 
      gr_sel->SetPointError(j,0,dx);
      j++; max_x=-1; max_y=-1;
    }
    if ((y>y_avg) && (min_y!=1)) {
      gr_sel->SetPoint(j, min_x, min_y);
      gr_sel->SetPointError(j,0,dx);
      j++; min_x=1; min_y=1;

    }

  }

  Double_t start_draw=0, end_draw=0;
  gr_sel->GetPoint(1, start_draw, temp);
  gr_sel->GetPoint(j-1, end_draw, temp);

  gr_sig->GetXaxis()->SetRangeUser(start_draw-0.001, end_draw+0.001);
  gr_sel->Draw("same p");

  char outfile[50];

  sprintf(outfile, "%s/cp_%i.png",dir.Data(),n);

  c1->SaveAs(outfile);

  delete c1;

}
#endif
