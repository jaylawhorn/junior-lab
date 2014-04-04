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
#include <TGaxis.h>
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

void magnetite() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);
  TGaxis::SetMaxDigits(4);

  TH1D *feVel = new TH1D("feVel", "feVel", 2048, 0, 2048);
  TH1D *feMag = new TH1D("feMag", "feMag", 1024, 0, 2048);

  confParse("data_list.txt", 5, feVel);
  confParse("data_list.txt", 13, feMag);

  feVel->SetTitle("");
  feVel->GetXaxis()->SetTitle("Channel");
  feVel->GetYaxis()->SetTitle("Counts");
  feVel->SetLineColor(kRed);
  feVel->Draw();

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

  //fitPeak->SetLineColor(kBlue);

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
  TF1 *velCurve2 = new TF1("velCurve2", "[0]*x+[1]", 200, peakPos[2]+50);
  TF1 *velCurve3 = new TF1("velCurve3", "[0]*x+[1]", peakPos[3]-50, 2040);
  getVel(peakPos, peakPosUnc, velCurve);
  getVel(peakPos, peakPosUnc, velCurve2);
  getVel(peakPos, peakPosUnc, velCurve3);

  velCurve->SetLineColor(kBlue);
  velCurve2->SetLineColor(kGreen);
  velCurve3->SetLineColor(kGreen);

  velCurve->Draw("same");
  velCurve2->Draw("same");
  velCurve3->Draw("same");

  c1->SaveAs("mag_velcurves.png");

  feMag->SetTitle("");
  feMag->GetXaxis()->SetTitle("Channel");
  feMag->GetXaxis()->SetNdivisions(8,5,0);
  feMag->GetYaxis()->SetTitle("Counts");
  feMag->SetLineColor(kRed);
  feMag->Draw();

  TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)-[13]*([15]/2)^2/((x-[14])^2+([15]/2)^2)-[16]*([18]/2)^2/((x-[17])^2+([18]/2)^2)-[19]*([21]/2)^2/((x-[20])^2+([21]/2)^2)-[22]*([24]/2)^2/((x-[23])^2+([24]/2)^2)-[25]*([27]/2)^2/((x-[26])^2+([27]/2)^2)-[28]*(x-[29])**2-[30]*([32]/2)^2/((x-[31])^2+([32]/2)^2)",150, 2040);

  TF1 *fitPeak3 = new TF1("fitPeak3", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)",500, 900);
  
  fitPeak2->SetParameter(0, 34000);
  fitPeak3->SetParameter(0, 34000);

  fitPeak2->SetParameter(1, 20);
  fitPeak2->SetParameter(2, 250);
  fitPeak2->SetParameter(3, 15);

  fitPeak2->SetParameter(4, 20);
  fitPeak2->SetParameter(5, 400);
  fitPeak2->SetParameter(6, 15);

  fitPeak2->SetParameter(7, 200);
  fitPeak2->SetParameter(8, 600);
  fitPeak2->SetParameter(9, 15);

  fitPeak2->SetParameter(10, 20);
  fitPeak2->SetParameter(11, 650);
  fitPeak2->SetParameter(12, 15);

  fitPeak3->SetParameter(1, 200);
  fitPeak3->SetParameter(2, 600);
  fitPeak3->SetParameter(3, 15);

  fitPeak3->SetParameter(4, 20);
  fitPeak3->SetParameter(5, 650);
  fitPeak3->SetParameter(6, 15);

  fitPeak2->SetParameter(13, 20);
  fitPeak2->SetParameter(14, 900);
  fitPeak2->SetParameter(15, 15);

  fitPeak2->SetParameter(16, 20);
  fitPeak2->SetParameter(17, 1100);
  fitPeak2->SetParameter(18, 15);

  fitPeak2->SetParameter(19, 30);
  fitPeak2->SetParameter(20, 1210);
  fitPeak2->SetParameter(21, 20);

  fitPeak2->SetParameter(22, 20);
  fitPeak2->SetParameter(23, 1600);
  fitPeak2->SetParameter(24, 15);

  fitPeak2->SetParameter(25, 20);
  fitPeak2->SetParameter(26, 2000);
  fitPeak2->SetParameter(27, 15);

  fitPeak2->SetParameter(28, 0);
  fitPeak2->SetParameter(29, 0);

  fitPeak2->SetParameter(30, 20);
  fitPeak2->SetParameter(31, 1130);
  fitPeak2->SetParameter(32, 10);

  feMag->Fit("fitPeak2", "RM");
  feMag->Fit("fitPeak3", "RN");
  //fitPeak3->Draw("same");

  c1->SaveAs("mag_peaks.png");

  vector<Double_t> magPeak;
  vector<Double_t> magPeakUnc1;
  vector<Double_t> magPeakUnc2;
  vector<Double_t> magPeakUnc;

  for (Int_t i=2; i<29; i+=3) {
    cout << i << " " <<  fitPeak2->GetParameter(i) << "\pm" << TMath::Sqrt(fabs(fitPeak2->GetParameter(i+1))) << endl;
  }
  cout << "31 " << fitPeak2->GetParameter(31) << "\pm" << TMath::Sqrt(fabs(fitPeak2->GetParameter(31+1))) << endl;

  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(2)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(5)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(11)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(14)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(31)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(20)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(23)));
  magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(26)));

  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[0]-velCurve->Eval(fitPeak2->GetParameter(2)+TMath::Sqrt(fabs(fitPeak2->GetParameter(3))))), fabs(magPeak[0]-velCurve->Eval(fitPeak2->GetParameter(2)-TMath::Sqrt(fabs(fitPeak2->GetParameter(3))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[1]-velCurve->Eval(fitPeak2->GetParameter(5)+TMath::Sqrt(fabs(fitPeak2->GetParameter(6))))), fabs(magPeak[1]-velCurve->Eval(fitPeak2->GetParameter(5)-TMath::Sqrt(fabs(fitPeak2->GetParameter(6))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[2]-velCurve->Eval(fitPeak2->GetParameter(11)+TMath::Sqrt(fabs(fitPeak2->GetParameter(12))))), fabs(magPeak[2]-velCurve->Eval(fitPeak2->GetParameter(11)-TMath::Sqrt(fabs(fitPeak2->GetParameter(12))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[3]-velCurve->Eval(fitPeak2->GetParameter(14)+TMath::Sqrt(fabs(fitPeak2->GetParameter(15))))), fabs(magPeak[3]-velCurve->Eval(fitPeak2->GetParameter(14)-TMath::Sqrt(fabs(fitPeak2->GetParameter(15))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[4]-velCurve->Eval(fitPeak2->GetParameter(31)+TMath::Sqrt(fabs(fitPeak2->GetParameter(32))))), fabs(magPeak[4]-velCurve->Eval(fitPeak2->GetParameter(31)-TMath::Sqrt(fabs(fitPeak2->GetParameter(32))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[5]-velCurve->Eval(fitPeak2->GetParameter(20)+TMath::Sqrt(fabs(fitPeak2->GetParameter(21))))), fabs(magPeak[5]-velCurve->Eval(fitPeak2->GetParameter(20)-TMath::Sqrt(fabs(fitPeak2->GetParameter(21))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[6]-velCurve->Eval(fitPeak2->GetParameter(23)+TMath::Sqrt(fabs(fitPeak2->GetParameter(24))))), fabs(magPeak[6]-velCurve->Eval(fitPeak2->GetParameter(23)-TMath::Sqrt(fabs(fitPeak2->GetParameter(24))))))); 
  magPeakUnc1.push_back(TMath::Max(fabs(magPeak[7]-velCurve->Eval(fitPeak2->GetParameter(26)+TMath::Sqrt(fabs(fitPeak2->GetParameter(27))))), fabs(magPeak[7]-velCurve->Eval(fitPeak2->GetParameter(26)-TMath::Sqrt(fabs(fitPeak2->GetParameter(27))))))); 

  magPeakUnc2.push_back(fabs(magPeak[0]-velCurve2->Eval(fitPeak2->GetParameter(2)))); 
  magPeakUnc2.push_back(fabs(magPeak[1]-velCurve2->Eval(fitPeak2->GetParameter(5)))); 
  magPeakUnc2.push_back(fabs(magPeak[2]-velCurve2->Eval(fitPeak2->GetParameter(11)))); 
  magPeakUnc2.push_back(fabs(magPeak[3]-velCurve2->Eval(fitPeak2->GetParameter(14)))); 
  magPeakUnc2.push_back(fabs(magPeak[4]-velCurve2->Eval(fitPeak2->GetParameter(31)))); 
  magPeakUnc2.push_back(fabs(magPeak[5]-velCurve2->Eval(fitPeak2->GetParameter(20)))); 
  magPeakUnc2.push_back(fabs(magPeak[6]-velCurve2->Eval(fitPeak2->GetParameter(23)))); 
  magPeakUnc2.push_back(fabs(magPeak[7]-velCurve2->Eval(fitPeak2->GetParameter(26)))); 

  for (Int_t i=0; i<magPeak.size(); i++) {
    magPeakUnc.push_back(TMath::Sqrt(magPeakUnc1[i]*magPeakUnc1[i]+magPeakUnc2[i]*magPeakUnc2[i]));

    cout << magPeak[i] << " +- " << magPeakUnc1[i] << " +- " << magPeakUnc2[i] << endl;

  }

  /*  for (Int_t i=2; i<6; i++) {
    magPeak.push_back(velCurve->Eval(fitPeak2->GetParameter(2+3*i)));

    magPeakUnc1.push_back(TMath::Max(fabs(magPeak[i]-velCurve->Eval(fitPeak2->GetParameter(2+3*i)+TMath::Sqrt(fabs(fitPeak2->GetParameter(3+3*i))))), fabs(magPeak[i]-velCurve->Eval(fitPeak2->GetParameter(2+3*i)-TMath::Sqrt(fabs(fitPeak2->GetParameter(3+3*i))))))); 

    if (i<3) {
      cout << velCurve2->Eval(fitPeak2->GetParameter(2+3*i)) << endl;
      magPeakUnc2.push_back(fabs(magPeak[i]-velCurve2->Eval(fitPeak2->GetParameter(2+3*i))));
    }
    else {
      cout << velCurve3->Eval(fitPeak2->GetParameter(2+3*i)) << endl;
      magPeakUnc2.push_back(fabs(magPeak[i]-velCurve3->Eval(fitPeak2->GetParameter(2+3*i))));
    }
    magPeakUnc.push_back(TMath::Sqrt(magPeakUnc1[i]*magPeakUnc1[i]+magPeakUnc2[i]*magPeakUnc2[i]));

    cout << magPeak[i] << " +- " << magPeakUnc1[i] << " +- " << magPeakUnc2[i] << endl;
  }

  //replace with weighted averages!

  Double_t dE0_1 = fabs(magPeak[3]-magPeak[1]);
  Double_t dE0_2 = fabs(magPeak[4]-magPeak[2]);
  Double_t dE0unc_1 = TMath::Sqrt(magPeakUnc[3]*magPeakUnc[3]+magPeakUnc[1]*magPeakUnc[1]);
  Double_t dE0unc_2 = TMath::Sqrt(magPeakUnc[4]*magPeakUnc[4]+magPeakUnc[2]*magPeakUnc[2]);
  Double_t dE0 = (dE0_1/(dE0unc_1*dE0unc_1)+dE0_2/(dE0unc_2*dE0unc_2))/(1/(dE0unc_1*dE0unc_1)+1/(dE0unc_2*dE0unc_2));
  Double_t dE0unc = TMath::Sqrt(1/(1/(dE0unc_1*dE0unc_1)+1/(dE0unc_2*dE0unc_2)));

  Double_t dE1_1 = fabs(magPeak[4]-magPeak[3]);
  Double_t dE1_2 = fabs(magPeak[2]-magPeak[1]);
  Double_t dE1unc_1 = TMath::Sqrt(magPeakUnc[3]*magPeakUnc[3]+magPeakUnc[4]*magPeakUnc[4]);
  Double_t dE1unc_2 = TMath::Sqrt(magPeakUnc[1]*magPeakUnc[1]+magPeakUnc[2]*magPeakUnc[2]);
  Double_t dE1 = (dE1_1/(dE1unc_1*dE1unc_1)+dE1_2/(dE1unc_2*dE1unc_2))/(1/(dE1unc_1*dE1unc_1)+1/(dE1unc_2*dE1unc_2));
  Double_t dE1unc = TMath::Sqrt(1/(1/(dE1unc_1*dE1unc_1)+1/(dE1unc_2*dE1unc_2)));

  Double_t q_1 = 0.5*fabs(fabs(magPeak[1]-magPeak[0])-dE1);
  Double_t q_2 = 0.5*fabs(fabs(magPeak[5]-magPeak[4])-dE1);
  Double_t qunc_1 = TMath::Sqrt(magPeakUnc[1]*magPeakUnc[1]+magPeakUnc[0]*magPeakUnc[0]+dE1unc*dE1unc);
  Double_t qunc_2 = TMath::Sqrt(magPeakUnc[5]*magPeakUnc[5]+magPeakUnc[4]*magPeakUnc[4]+dE1unc*dE1unc);
  Double_t q = (q_1/(qunc_1*qunc_1)+q_2/(qunc_2*qunc_2))/(1/(qunc_1*qunc_1)+1/(qunc_2*qunc_2));
  Double_t qunc = TMath::Sqrt(1/(1/(qunc_1*qunc_1)+1/(qunc_2*qunc_2)));

  Double_t nomE=14.4e3;
  Double_t cLight=3e11;

  cout << "Fe2O3 deltaV [mm/s]" << endl;
  cout << "dE1+2Q  = " << fabs(magPeak[1]-magPeak[0]) << endl;
  cout << "dE1     = " << fabs(magPeak[4]-magPeak[3]) << endl;
  cout << "dE1     = " << fabs(magPeak[2]-magPeak[1]) << endl;
  cout << "dE1-2Q  = " << fabs(magPeak[5]-magPeak[4]) << endl;
  cout << "----------" << endl;
  cout << "dE0     = " << dE0 << " +- " << dE0unc << endl;
  cout << "dE1     = " << dE1 << " +- " << dE1unc << endl;
  cout << "q       = " << q << " +- " << qunc << endl;
  cout << "deltaE [eV]" << endl;
  cout << "dE0     = " << dE0*nomE/cLight << " +- " << dE0unc*nomE/cLight << endl;
  cout << "dE1     = " << dE1*nomE/cLight << " +- " << dE1unc*nomE/cLight << endl;
  cout << "q       = " << q*nomE/cLight << " +- " << qunc*nomE/cLight << endl;

  cout << "mu1/mu0 = " << -3*dE1/dE0 << " +- " << -3*dE1/dE0*TMath::Sqrt((dE0unc*dE0unc)/(dE0*dE0)+(dE1unc*dE1unc)/(dE1*dE1)) << endl;

    */
}
