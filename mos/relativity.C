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

void extractValues(TF1* velCurve, TF1* velCurve2, TF1* velCurveL, TF1* velCurveR, TF1* fitPeak);

void relativity() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  //128
  TH1D *feVel1 = new TH1D("feVel1", "feVel1", 2048, 0, 2048);
  TH1D *feCold1 = new TH1D("feCold1", "feCold1", 2048, 0, 2048);
  TH1D *feHot = new TH1D("feHot", "feHot", 2048, 0, 2048);
  TH1D *feVel2 = new TH1D("feVel2", "feVel2", 2048, 0, 2048);
  TH1D *feCold2 = new TH1D("feCold2", "feCold2", 2048, 0, 2048);

  confParse("data_list.txt", 8, feVel1);
  confParse("data_list.txt", 9, feCold1);
  confParse("data_list.txt", 10, feHot);
  confParse("data_list.txt", 11, feVel2);
  confParse("data_list.txt", 12, feCold2);

  feVel1->SetTitle("");
  feVel1->GetXaxis()->SetTitle("Channel");
  feVel1->GetYaxis()->SetTitle("Counts");
  feVel1->SetLineColor(kRed);
  feVel1->Draw();

  TF1 *fitPeak1 = new TF1("fitPeak1", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)",150, 2040);

  fitPeak1->SetParameter(0, 360);
  fitPeak1->SetParameter(1, 20);
  fitPeak1->SetParameter(2, 450);
  fitPeak1->SetParameter(3, 10);
  fitPeak1->SetParameter(4, 20);
  fitPeak1->SetParameter(5, 850);
  fitPeak1->SetParameter(6, 10);
  fitPeak1->SetParameter(7, 200);
  fitPeak1->SetParameter(8, 1190);
  fitPeak1->SetParameter(9, 10);
  fitPeak1->SetParameter(10, 20);
  fitPeak1->SetParameter(11, 1650);
  fitPeak1->SetParameter(12, 10);
  fitPeak1->SetLineColor(kBlue);
  feVel1->Fit("fitPeak1", "R");
  c1->SaveAs("relativity_calibration1.png");

  vector<Double_t> peakPos1;
  vector<Double_t> peakPosUnc1;

  peakPos1.push_back(fitPeak1->GetParameter(2));  peakPosUnc1.push_back(TMath::Sqrt(fabs(fitPeak1->GetParameter(3))));
  peakPos1.push_back(fitPeak1->GetParameter(5));  peakPosUnc1.push_back(TMath::Sqrt(fabs(fitPeak1->GetParameter(6))));
  peakPos1.push_back(fitPeak1->GetParameter(8));  peakPosUnc1.push_back(TMath::Sqrt(fabs(fitPeak1->GetParameter(9))));
  peakPos1.push_back(fitPeak1->GetParameter(11)); peakPosUnc1.push_back(TMath::Sqrt(fabs(fitPeak1->GetParameter(12))));

  for (Int_t i=0; i<4; i++) {
    cout << peakPos1[i] << endl;
  }

  TF1 *velCurve1 = new TF1("velCurve1", "[0]*x+[1]", 200, 2040);
  TF1 *velCurve1test = new TF1("velCurve1test", "[0]*x+[1]+[2]*x^2", 200, 2040);
  TF1 *velCurveL1 = new TF1("velCurveL1", "[0]*x+[1]", 200, 1100);
  TF1 *velCurveR1 = new TF1("velCurveR1", "[0]*x+[1]", 900, 2040);
  getVel(peakPos1, peakPosUnc1, velCurve1);
  getVel(peakPos1, peakPosUnc1, velCurve1test);
  getVel(peakPos1, peakPosUnc1, velCurveL1);
  getVel(peakPos1, peakPosUnc1, velCurveR1);

  velCurve1->SetLineColor(kBlue);
  velCurve1test->SetLineColor(kGreen);

  velCurve1->Draw("same");
  velCurve1test->Draw("same");
  c1->SaveAs("relativity_velcurves1.png");

  cout << "Velocity curve 1: " << velCurve1->GetParameter(0) << "*x + " << velCurve1->GetParameter(1) << endl;
  cout << "Velocity curve test: " << velCurve1test->GetParameter(0) << "*x + " << velCurve1test->GetParameter(1) << " + " << velCurve1test->GetParameter(2) << "*x^2 " << endl;
  cout << "unc par 0 " << velCurve1->GetParError(0) << " par 1 " << velCurve1->GetParError(1) << endl;
  cout << velCurve1->Eval(2000) << " - " << velCurve1test->Eval(2000) << " " << velCurve1->Eval(2000)-velCurve1test->Eval(2000) << endl;
  cout << velCurve1->Eval(10) << " - " << velCurve1test->Eval(10) << " " << velCurve1->Eval(10)-velCurve1test->Eval(10) << endl;

  feVel2->SetTitle("");
  feVel2->GetXaxis()->SetTitle("Channel");
  feVel2->GetYaxis()->SetTitle("Counts");
  feVel2->SetLineColor(kRed);
  feVel2->Draw();

  TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)",150, 2040);

  fitPeak2->SetParameter(0, 360);
  fitPeak2->SetParameter(1, 20);
  fitPeak2->SetParameter(2, 450);
  fitPeak2->SetParameter(3, 10);
  fitPeak2->SetParameter(4, 20);
  fitPeak2->SetParameter(5, 850);
  fitPeak2->SetParameter(6, 10);
  fitPeak2->SetParameter(7, 200);
  fitPeak2->SetParameter(8, 1190);
  fitPeak2->SetParameter(9, 10);
  fitPeak2->SetParameter(10, 20);
  fitPeak2->SetParameter(11, 1650);
  fitPeak2->SetParameter(12, 100);
  fitPeak2->SetLineColor(kBlue);
  feVel2->Fit("fitPeak2", "R");
  c1->SaveAs("relativity_calibration2.png");

  vector<Double_t> peakPos2;
  vector<Double_t> peakPosUnc2;

  peakPos2.push_back(fitPeak2->GetParameter(2));  peakPosUnc2.push_back(TMath::Sqrt(fabs(fitPeak2->GetParameter(3))));
  peakPos2.push_back(fitPeak2->GetParameter(5));  peakPosUnc2.push_back(TMath::Sqrt(fabs(fitPeak2->GetParameter(6))));
  peakPos2.push_back(fitPeak2->GetParameter(8));  peakPosUnc2.push_back(TMath::Sqrt(fabs(fitPeak2->GetParameter(9))));
  peakPos2.push_back(fitPeak2->GetParameter(11)); peakPosUnc2.push_back(TMath::Sqrt(fabs(fitPeak2->GetParameter(12))));

  TF1 *velCurve2 = new TF1("velCurve2", "[0]*x+[1]", 200, 2040);
  TF1 *velCurve2test = new TF1("velCurve2test", "[0]*x+[1]+[2]*x^2", 200, 2040);
  TF1 *velCurveL2 = new TF1("velCurveL2", "[0]*x+[1]", 200, 1100);
  TF1 *velCurveR2 = new TF1("velCurveR2", "[0]*x+[1]", 900, 2040);
  getVel(peakPos2, peakPosUnc2, velCurve2);
  getVel(peakPos2, peakPosUnc2, velCurve2test);
  getVel(peakPos2, peakPosUnc2, velCurveL2);
  getVel(peakPos2, peakPosUnc2, velCurveR2);

  velCurve2->SetLineColor(kBlue);
  velCurve2test->SetLineColor(kGreen);

  velCurve2->Draw("same");
  velCurve2test->Draw("same");
  c1->SaveAs("relativity_velcurves2.png");

  cout << "Velocity curve 2: " << velCurve2->GetParameter(0) << "*x + " << velCurve2->GetParameter(1) << endl;
  cout << "Velocity curve test: " << velCurve2test->GetParameter(0) << "*x + " << velCurve2test->GetParameter(1) << " + " << velCurve2test->GetParameter(2) << "*x^2 " << endl;
  cout << "unc par 0 " << velCurve2->GetParError(0) << " par 1 " << velCurve2->GetParError(1) << endl;
  cout << velCurve2->Eval(2000) << " - " << velCurve2test->Eval(2000) << " " << velCurve2->Eval(2000)-velCurve2test->Eval(2000) << endl;
  cout << velCurve2->Eval(10) << " - " << velCurve2test->Eval(10) << " " << velCurve2->Eval(10)-velCurve2test->Eval(10) << endl;

  feCold1->SetTitle("");
  feCold1->GetXaxis()->SetTitle("Channel");
  feCold1->GetYaxis()->SetTitle("Counts");
  feCold1->SetLineColor(kBlack);
  feCold1->Draw();

  TF1 *fitCold1 = new TF1("fitCold1", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*(x-[5])**2",150, 2040);
 
  fitCold1->SetParameter(0, 340);
  fitCold1->SetParameter(1, 20);
  fitCold1->SetParameter(2, 1050);
  fitCold1->SetParameter(3, 50);
  fitCold1->SetLineColor(kBlue);
  feCold1->Fit("fitCold1", "RM");

  c1->SaveAs("cold_line1.png");

  feHot->SetTitle("");
  feHot->GetXaxis()->SetTitle("Channel");
  feHot->GetYaxis()->SetTitle("Counts");
  feHot->SetLineColor(kBlack);
  feHot->Draw();

  TF1 *fitHot = new TF1("fitHot", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*(x-[5])**2",150, 2040);
 
  fitHot->SetParameter(0, 2200);
  fitHot->SetParameter(1, 20);
  fitHot->SetParameter(2, 1050);
  fitHot->SetParameter(3, 100);
  fitHot->SetLineColor(kRed);
  feHot->Fit("fitHot", "RM");

  c1->SaveAs("hot_line.png");

  feCold2->SetTitle("");
  feCold2->GetXaxis()->SetTitle("Channel");
  feCold2->GetYaxis()->SetTitle("Counts");
  feCold2->SetLineColor(kBlack);
  feCold2->Draw();

  TF1 *fitCold2 = new TF1("fitCold2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*(x-[5])**2",150, 2040);
 
  fitCold2->SetParameter(0, 4300);
  fitCold2->SetParameter(1, 20);
  fitCold2->SetParameter(2, 1050);
  fitCold2->SetParameter(3, 50);
  fitCold2->SetLineColor(kBlue);
  feCold2->Fit("fitCold2", "RM");

  c1->SaveAs("cold_line2.png");

  cout << "Cold #1: " << endl;
  extractValues(velCurve1, velCurve1test, velCurveL1, velCurveR1, fitCold1);
  cout << "Hot: " << endl;
  extractValues(velCurve1, velCurve1test, velCurveL1, velCurveR1, fitHot);
  cout << "Cold #2: " << endl;
  extractValues(velCurve2, velCurve2test, velCurveL2, velCurveR2, fitCold2);

}

void extractValues(TF1* velCurve, TF1* velCurve2, TF1* velCurveL, TF1* velCurveR, TF1* fitPeak) {

  Double_t intercept =fabs((velCurveL->GetParameter(1)-velCurveR->GetParameter(1))/(velCurveL->GetParameter(0)-velCurveR->GetParameter(0)));

  Double_t vel_0 = velCurve->Eval(fitPeak->GetParameter(2));
  Double_t dvel_0 = velCurve->Eval(fitPeak->GetParameter(2)+TMath::Sqrt(fitPeak->GetParameter(3)))-vel_0;
  Double_t vel_1 = velCurve2->Eval(fitPeak->GetParameter(2));
  Double_t dvel_1 = velCurve2->Eval(fitPeak->GetParameter(2)+TMath::Sqrt(fitPeak->GetParameter(3)))-vel_1;
  Double_t vel_2;
  Double_t dvel_2;
  if (fitPeak->GetParameter(2)<intercept){
    vel_2 = velCurveL->Eval(fitPeak->GetParameter(2));
    dvel_2 = velCurveL->Eval(fitPeak->GetParameter(2)+TMath::Sqrt(fitPeak->GetParameter(3)))-vel_2;
  } 
  else {
    vel_2 = velCurveR->Eval(fitPeak->GetParameter(2));
    dvel_2 =  velCurveR->Eval(fitPeak->GetParameter(2)+TMath::Sqrt(fitPeak->GetParameter(3)))-vel_2;
  }
  cout << "velocity estimates: " << vel_0 << ", " << vel_1 << ", " << vel_2 << endl;
  cout << "                    " << dvel_0 << ", " << dvel_1 << ", " << dvel_2 << endl;
  
  Double_t avg_vel = (vel_0+vel_1+vel_2)/3.; 
  Double_t avg_dvel = (dvel_0+dvel_1+dvel_2)/3.; 
  Double_t stdev_vel = TMath::Sqrt((vel_0*vel_0+vel_1*vel_1+vel_2*vel_2)/3.-(avg_vel*avg_vel));
  Double_t stdev_dvel = TMath::Sqrt((dvel_0*dvel_0+dvel_1*dvel_1+dvel_2*dvel_2)/3.-(avg_dvel*avg_dvel));
  cout << avg_vel << " +- " << stdev_vel/TMath::Sqrt(3) << " +- " << avg_dvel << endl;
  cout << avg_vel << " +- " << TMath::Sqrt(stdev_vel*stdev_vel/3+avg_dvel*avg_dvel) << endl;
  cout << endl;

}
