//----------------------------------------------------------------------------
//
// Test of secondary velocity calibration code - expect to regain numbers
// from primary velocity calibration code. This pans out!
// (3/12/14)
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

void velocity_test() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  TH1D *fe = new TH1D("fe", "fe", 2048, 0, 2048);

  confParse("data_list.txt", 4, fe);

  fe->SetTitle("");
  fe->GetXaxis()->SetTitle("Channel");
  fe->GetYaxis()->SetTitle("Counts");
  fe->SetLineColor(kRed);
  fe->Draw();

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

  fe->Fit("fitPeak", "R");

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

  vector<Double_t> vel;
  vector<Double_t> velunc;

  vel.push_back(-6.18); velunc.push_back(0.17);
  vel.push_back(-3.63); velunc.push_back(0.16);
  vel.push_back(-1.10); velunc.push_back(0.11);
  vel.push_back(0.83); velunc.push_back(0.08);
  vel.push_back(3.37); velunc.push_back(0.08);
  vel.push_back(5.92); velunc.push_back(0.11);


  for (Int_t i=0; i<6; i++) {
    cout << velCurve->Eval(peakPos[i]) << " " << vel[i] << " " << 100*(velCurve->Eval(peakPos[i])-vel[i])/velCurve->Eval(peakPos[i]) << endl;
    cout << getVelUnc(velCurve->Eval(peakPos[i])) << endl;
  }
  
}
