//----------------------------------------------------------------------------
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

void fe2o3() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  TH1D *feVel = new TH1D("feVel", "feVel", 2048, 0, 2048);
  TH1D *feQuad = new TH1D("feQuad", "feQuad", 2048, 0, 2048);

  confParse("data_list.txt", 5, feVel);
  confParse("data_list.txt", 6, feQuad);

  feVel->SetTitle("");
  feVel->GetXaxis()->SetTitle("Channel");
  feVel->GetYaxis()->SetTitle("Counts");
  feVel->SetLineColor(kRed);
  //feVel->Draw();

  TF1 *fitPeak = new TF1("fitPeak", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)-[13]*([15]/2)^2/((x-[14])^2+([15]/2)^2)-[16]*([18]/2)^2/((x-[17])^2+([18]/2)^2)-[19]*(x-[20])**2",200, 2040);

  fitPeak->SetParameter(0, 120);

  fitPeak->SetParameter(1, 20);
  fitPeak->SetParameter(2, 500);
  fitPeak->SetParameter(3, 15);

  fitPeak->SetParameter(4, 20);
  fitPeak->SetParameter(5, 700);
  fitPeak->SetParameter(6, 15);

  fitPeak->SetParameter(7, 200);
  fitPeak->SetParameter(8, 1000);
  fitPeak->SetParameter(9, 15);

  fitPeak->SetParameter(10, 20);
  fitPeak->SetParameter(11, 1100);
  fitPeak->SetParameter(12, 15);

  fitPeak->SetParameter(13, 20);
  fitPeak->SetParameter(14, 1400);
  fitPeak->SetParameter(15, 15);

  fitPeak->SetParameter(16, 20);
  fitPeak->SetParameter(17, 1600);
  fitPeak->SetParameter(18, 15);

  fitPeak->SetLineColor(kBlue);

  feVel->Fit("fitPeak", "RN");

  vector<Double_t> peakPos;
  vector<Double_t> peakPosUnc;

  peakPos.push_back(fitPeak->GetParameter(2));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(3))));
  peakPos.push_back(fitPeak->GetParameter(5));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(6))));
  peakPos.push_back(fitPeak->GetParameter(8));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(9))));
  peakPos.push_back(fitPeak->GetParameter(11)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(12))));
  peakPos.push_back(fitPeak->GetParameter(14)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(15))));
  peakPos.push_back(fitPeak->GetParameter(17)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(18))));

  TF1 *velCurve = new TF1("velCurve", "[0]*x+[1]*x^2+[2]", 200, 2040);
  getVel(peakPos, peakPosUnc, velCurve);

  feQuad->SetTitle("");
  feQuad->GetXaxis()->SetTitle("Channel");
  feQuad->GetYaxis()->SetTitle("Counts");
  feQuad->SetLineColor(kRed);
  feQuad->Draw();

  TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)-[13]*([15]/2)^2/((x-[14])^2+([15]/2)^2)-[16]*([18]/2)^2/((x-[17])^2+([18]/2)^2)-[19]*(x-[20])**2",150, 2040);

  fitPeak2->SetParameter(0, 34000);

  fitPeak2->SetParameter(1, 20);
  fitPeak2->SetParameter(2, 250);
  fitPeak2->SetParameter(3, 15);

  fitPeak2->SetParameter(4, 20);
  fitPeak2->SetParameter(5, 600);
  fitPeak2->SetParameter(6, 15);

  fitPeak2->SetParameter(7, 200);
  fitPeak2->SetParameter(8, 900);
  fitPeak2->SetParameter(9, 15);

  fitPeak2->SetParameter(10, 20);
  fitPeak2->SetParameter(11, 1200);
  fitPeak2->SetParameter(12, 15);

  fitPeak2->SetParameter(13, 20);
  fitPeak2->SetParameter(14, 1600);
  fitPeak2->SetParameter(15, 15);

  fitPeak2->SetParameter(16, 20);
  fitPeak2->SetParameter(17, 2000);
  fitPeak2->SetParameter(18, 15);

  fitPeak2->SetLineColor(kBlue);

  feQuad->Fit("fitPeak2", "R");

  vector<Double_t> feoPeak;
  vector<Double_t> feoPeakUnc1;
  vector<Double_t> feoPeakUnc2;
  vector<Double_t> feoPeakUnc;

  for (Int_t i=0; i<6; i++) {
    feoPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(2+3*i)));

    feoPeakUnc1.push_back(TMath::Max(fabs(feoPeak[i]-velCurve->Eval(fitPeak2->GetParameter(2+3*i)+TMath::Sqrt(fabs(fitPeak2->GetParameter(3+3*i))))), fabs(feoPeak[i]-velCurve->Eval(fitPeak2->GetParameter(2+3*i)-TMath::Sqrt(fabs(fitPeak2->GetParameter(3+3*i))))))); 
    //feoPeakUnc2.push_back(fabs(feoPeak[i]-velCurve2->Eval(fitPeak2->GetParameter(2+3*i))));
    feoPeakUnc2.push_back(0);

    cout << feoPeak[i] << " +- " << feoPeakUnc1[i] << " +- " << feoPeakUnc2[i] << endl;

    feoPeakUnc.push_back(TMath::Sqrt(feoPeakUnc1[i]*feoPeakUnc1[i]+feoPeakUnc2[i]*feoPeakUnc2[i]));
  }

  //replace with weighted averages!

  Double_t dE0 = 0.5*(fabs(feoPeak[3]-feoPeak[1])+fabs(feoPeak[4]-feoPeak[2]));
  Double_t dE0unc = TMath::Sqrt(feoPeakUnc[3]*feoPeakUnc[3]+feoPeakUnc[1]*feoPeakUnc[1]+feoPeakUnc[4]*feoPeakUnc[4]+feoPeakUnc[2]*feoPeakUnc[2]);

  Double_t dE1 = 0.5*(fabs(feoPeak[4]-feoPeak[3])+fabs(feoPeak[2]-feoPeak[1]));
  Double_t dE1unc = TMath::Sqrt(feoPeakUnc[3]*feoPeakUnc[3]+feoPeakUnc[1]*feoPeakUnc[1]+feoPeakUnc[4]*feoPeakUnc[4]+feoPeakUnc[2]*feoPeakUnc[2]);

  Double_t q = 0.5*0.5*(fabs(feoPeak[1]-feoPeak[0])-fabs(feoPeak[5]-feoPeak[4]));
  Double_t qunc = TMath::Sqrt(feoPeakUnc[0]*feoPeakUnc[0]+feoPeakUnc[1]*feoPeakUnc[1]+feoPeakUnc[4]*feoPeakUnc[4]+feoPeakUnc[5]*feoPeakUnc[5]);

  Double_t nomE=14.4e3;
  Double_t cLight=3e11;

  cout << "Fe2O3 deltaV [mm/s]" << endl;
  cout << "dE1+2Q  = " << fabs(feoPeak[1]-feoPeak[0]) << endl;
  cout << "dE1     = " << fabs(feoPeak[4]-feoPeak[3]) << endl;
  cout << "dE1     = " << fabs(feoPeak[2]-feoPeak[1]) << endl;
  cout << "dE1-2Q  = " << fabs(feoPeak[5]-feoPeak[4]) << endl;
  cout << "----------" << endl;
  cout << "dE0     = " << dE0 << " +- " << dE0unc << endl;
  cout << "dE1     = " << dE1 << " +- " << dE1unc << endl;
  cout << "q       = " << q << " +- " << qunc << endl;
  cout << "deltaE [eV]" << endl;
  cout << "dE0     = " << dE0*nomE/cLight << " +- " << dE0unc*nomE/cLight << endl;
  cout << "dE1     = " << dE1*nomE/cLight << " +- " << dE1unc*nomE/cLight << endl;
  cout << "q       = " << q*nomE/cLight << " +- " << qunc*nomE/cLight << endl;

}
