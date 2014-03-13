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

void relativity() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  TH1D *feVel = new TH1D("feVel", "feVel", 256, 0, 2048);
  TH1D *feCold = new TH1D("feCold", "feCold", 256, 0, 2048);

  confParse("data_list.txt", 8, feVel);
  confParse("data_list.txt", 9, feCold);

  feVel->SetTitle("");
  feVel->GetXaxis()->SetTitle("Channel");
  feVel->GetYaxis()->SetTitle("Counts");
  feVel->SetLineColor(kRed);
  feVel->Draw();

  TF1 *fitPeak = new TF1("fitPeak", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)",150, 2040);

  fitPeak->SetParameter(0, 360);

  fitPeak->SetParameter(1, 20);
  fitPeak->SetParameter(2, 450);
  fitPeak->SetParameter(3, 10);

  fitPeak->SetParameter(4, 20);
  fitPeak->SetParameter(5, 850);
  fitPeak->SetParameter(6, 10);

  fitPeak->SetParameter(7, 200);
  fitPeak->SetParameter(8, 1190);
  fitPeak->SetParameter(9, 10);

  fitPeak->SetParameter(10, 20);
  fitPeak->SetParameter(11, 1650);
  fitPeak->SetParameter(12, 10);

  fitPeak->SetLineColor(kBlue);

  feVel->Fit("fitPeak", "R");

  vector<Double_t> peakPos;
  vector<Double_t> peakPosUnc;

  peakPos.push_back(fitPeak->GetParameter(2));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(3))));
  peakPos.push_back(fitPeak->GetParameter(5));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(6))));
  peakPos.push_back(fitPeak->GetParameter(8));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(9))));
  peakPos.push_back(fitPeak->GetParameter(11)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(12))));

  TF1 *velCurve = new TF1("velCurve", "[0]*x+[1]", 200, 2040);
  TF1 *velCurve2 = new TF1("velCurve2", "[0]*x+[1]", 200, 1100);
  TF1 *velCurve3 = new TF1("velCurve3", "[0]*x+[1]", 900, 2040);
  getVel(peakPos, peakPosUnc, velCurve);
  getVel(peakPos, peakPosUnc, velCurve2);
  getVel(peakPos, peakPosUnc, velCurve3);

  velCurve->SetLineColor(kBlue);
  velCurve2->SetLineColor(kGreen);
  velCurve3->SetLineColor(kGreen);

  velCurve->Draw("same");
  velCurve2->Draw("same");
  velCurve3->Draw("same");

  c1->SaveAs("relativity_velcurves.png");

  feCold->SetTitle("");
  feCold->GetXaxis()->SetTitle("Channel");
  feCold->GetYaxis()->SetTitle("Counts");
  feCold->SetLineColor(kRed);
  feCold->Draw();

  TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*(x-[5])**2",150, 2040);

  fitPeak2->SetParameter(0, 340);

  fitPeak2->SetParameter(1, 20);
  fitPeak2->SetParameter(2, 1050);
  fitPeak2->SetParameter(3, 15);

  fitPeak2->SetLineColor(kBlue);

  feCold->Fit("fitPeak2", "R");

  c1->SaveAs("relativity_peaks.png");

  Double_t nomPeak = velCurve->Eval(fitPeak2->GetParameter(2));
  Double_t peak2 = velCurve2->Eval(fitPeak2->GetParameter(2));
  Double_t peak3 = velCurve3->Eval(fitPeak2->GetParameter(2));

  cout << nomPeak << " " << peak2 << " " << peak3 << endl;

  cout << fitPeak2->GetParameter(2) << " +- " << TMath::Sqrt(fabs(fitPeak2->GetParameter(3)))*TMath::Sqrt(2048./256) << endl;
  cout << nomPeak << " +- " << velCurve->Eval(fitPeak2->GetParameter(2)+TMath::Sqrt(fabs(fitPeak2->GetParameter(3)))*TMath::Sqrt(2048./256)) << " +- " << TMath::Max(fabs(peak2-nomPeak), fabs(peak3-nomPeak)) << endl;
  /*
  vector<Double_t> feoPeak;
  vector<Double_t> feoPeakUnc1;
  vector<Double_t> feoPeakUnc2;
  vector<Double_t> feoPeakUnc;

  for (Int_t i=0; i<6; i++) {
    feoPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(2+3*i)));

    feoPeakUnc1.push_back(TMath::Max(fabs(feoPeak[i]-velCurve->Eval(fitPeak2->GetParameter(2+3*i)+TMath::Sqrt(fabs(fitPeak2->GetParameter(3+3*i))))), fabs(feoPeak[i]-velCurve->Eval(fitPeak2->GetParameter(2+3*i)-TMath::Sqrt(fabs(fitPeak2->GetParameter(3+3*i))))))); 

    if (i<3) {
      cout << velCurve2->Eval(fitPeak2->GetParameter(2+3*i)) << endl;
      feoPeakUnc2.push_back(fabs(feoPeak[i]-velCurve2->Eval(fitPeak2->GetParameter(2+3*i))));
    }
    else {
      cout << velCurve3->Eval(fitPeak2->GetParameter(2+3*i)) << endl;
      feoPeakUnc2.push_back(fabs(feoPeak[i]-velCurve3->Eval(fitPeak2->GetParameter(2+3*i))));
    }
    feoPeakUnc.push_back(TMath::Sqrt(feoPeakUnc1[i]*feoPeakUnc1[i]+feoPeakUnc2[i]*feoPeakUnc2[i]));

    cout << feoPeak[i] << " +- " << feoPeakUnc1[i] << " +- " << feoPeakUnc2[i] << endl;
  }
  */  

}
