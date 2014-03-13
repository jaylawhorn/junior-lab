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

TF1 *fitParts1, *fitParts2;

Double_t diff(double *x, double *par) {
  return TMath::Abs(fitParts1->EvalPar(x,par) - fitParts2->EvalPar(x,par));
}

void vel_cal_1() {

  Int_t upLim = 2048;
  Int_t loLim = 275;

  Int_t nPasses = 3117; //from absolute_cal...., abs_cal_1, abs_cal_2 only
  Float_t dwell = 0.1; //ms

  Float_t lambda = 0.6328; //micron need wavelength of laser!

  Double_t nomE=14.3e3; //eV
  Double_t cLight=3e11; //mm/s

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  TH1D *inf = new TH1D("inf", "inf", 2048, 0, 2048);  
  TH1D *fe = new TH1D("fe", "fe", 2048, 0, 2048);

  confParse("data_list.txt", 1, inf);
  confParse("data_list.txt", 2, fe);

  inf->SetTitle("");
  inf->GetXaxis()->SetTitle("Channel");
  inf->GetYaxis()->SetTitle("Counts");

  inf->Draw();
  fe->SetLineColor(kRed);
  fe->Draw("same");

  // fit to interferometric stuff
  TF1 *fitNom = new TF1("fitNom", "[0]+abs([1]*(x-[2])+[3]*(x-[2])**2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)", loLim, upLim);

  fitParts1 = new TF1("fitParts1", "[0]+[1]*x", 500, 1040);
  fitParts2 = new TF1("fitParts2", "[0]+[1]*x", 1080, 1600);

  fitNom->SetParameter(0, 250);
  fitNom->SetParameter(1, 10);
  fitNom->SetParameter(2, 1060);
  fitNom->SetParameter(4, 2000);
  fitNom->SetParameter(5, 410);
  fitNom->SetParameter(6, 25);

  TF1 *fitPeak = new TF1("fitPeak", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)-[13]*([15]/2)^2/((x-[14])^2+([15]/2)^2)-[16]*([18]/2)^2/((x-[17])^2+([18]/2)^2)-[19]*(x-[20])**2", loLim, upLim);

  fitPeak->SetParameter(0, 2600);

  fitPeak->SetParameter(1, 1000);
  fitPeak->SetParameter(2, 520);
  fitPeak->SetParameter(3, 25);

  fitPeak->SetParameter(4, 1000);
  fitPeak->SetParameter(5, 700);
  fitPeak->SetParameter(6, 25);

  fitPeak->SetParameter(7, 1000);
  fitPeak->SetParameter(8, 950);
  fitPeak->SetParameter(9, 25);

  fitPeak->SetParameter(10, 1000);
  fitPeak->SetParameter(11, 1150);
  fitPeak->SetParameter(12, 25);

  fitPeak->SetParameter(13, 1000);
  fitPeak->SetParameter(14, 1350);
  fitPeak->SetParameter(15, 25);

  fitPeak->SetParameter(16, 1000);
  fitPeak->SetParameter(17, 1600);
  fitPeak->SetParameter(18, 25);

  fitNom->SetLineColor(kBlue);
  fitParts1->SetLineColor(kGreen);
  fitParts2->SetLineColor(kGreen);
  fitPeak->SetLineColor(kBlue);

  inf->Fit("fitNom", "RN");
  inf->Fit("fitParts1", "RN");
  inf->Fit("fitParts2", "RN");
  fe->Fit("fitPeak", "RN");
  
  fitParts1->Draw("same");
  fitParts2->Draw("same");
  fitNom->Draw("same");
  fitPeak->Draw("same");
  
  Double_t b = fitNom->GetParameter(1);
  Double_t c = fitNom->GetParameter(2);
  Double_t d = fitNom->GetParameter(3);

  TF1 *velNom = new TF1("velNom", "abs([0]*(x-[1])+[2]*(x-[1])**2)", loLim, upLim);
  velNom->SetParameter(0, b*lambda/(2*nPasses*dwell));
  velNom->SetParameter(1, c);
  velNom->SetParameter(2, d*lambda/(2*nPasses*dwell));

  TF1 *diffPart = new TF1("diffPart", diff, loLim, upLim, 0);
  b = diffPart->GetMinimumX();

  TF1 *velPart1 = new TF1("velPart1", "[0]*(x-[1])+[2]", loLim, upLim);
  velPart1->SetParameter(0,fitParts1->GetParameter(1)*lambda/(2*nPasses*dwell));
  velPart1->SetParameter(1, b);
  velPart1->SetParameter(2,fitParts1->Eval(b)*lambda/(2*nPasses*dwell));

  TF1 *velPart2 = new TF1("velPart2", "[0]*(x-[1])-[2]", loLim, upLim);
  velPart2->SetParameter(0,fitParts2->GetParameter(1)*lambda/(2*nPasses*dwell));
  velPart2->SetParameter(1, b);
  velPart2->SetParameter(2,fitParts2->Eval(b)*lambda/(2*nPasses*dwell));
  
  vector<Double_t> peakPos;
  vector<Double_t> peakPosUnc;

  peakPos.push_back(fitPeak->GetParameter(2));  peakPosUnc.push_back(TMath::Sqrt(fitPeak->GetParameter(3)));
  peakPos.push_back(fitPeak->GetParameter(5));  peakPosUnc.push_back(TMath::Sqrt(fitPeak->GetParameter(6)));
  peakPos.push_back(fitPeak->GetParameter(8));  peakPosUnc.push_back(TMath::Sqrt(fitPeak->GetParameter(9)));
  peakPos.push_back(fitPeak->GetParameter(11)); peakPosUnc.push_back(TMath::Sqrt(fitPeak->GetParameter(12)));
  peakPos.push_back(fitPeak->GetParameter(14)); peakPosUnc.push_back(TMath::Sqrt(fitPeak->GetParameter(15)));
  peakPos.push_back(fitPeak->GetParameter(17)); peakPosUnc.push_back(TMath::Sqrt(fitPeak->GetParameter(18)));

  vector<Double_t> peakVel;
  vector<Double_t> peakVelUnc;
  vector<Double_t> peakVelVelUnc;
  vector<Double_t> peakVelPosUnc;

  cout << "    Peak Channel   Uncert" << endl;
  for (Int_t i=0; i<6; i++) {
    cout << "Peak " << i << ": " << peakPos[i] << " +- " << peakPosUnc[i] << endl;

    peakVel.push_back(velNom->Eval(peakPos[i]));
    if (i<3) {
      peakVelVelUnc.push_back(fabs(velNom->Eval(peakPos[i])-velPart1->Eval(peakPos[i])));
    }
    else {      
      peakVelVelUnc.push_back(fabs(velNom->Eval(peakPos[i])-velPart2->Eval(peakPos[i]))); 
    }
    peakVelPosUnc.push_back(TMath::Max(fabs(velNom->Eval(peakPos[i])-velNom->Eval(peakPos[i]+peakPosUnc[i])), fabs(velNom->Eval(peakPos[i])-velNom->Eval(peakPos[i]-peakPosUnc[i]))));
    
  }
  cout << endl;

  cout << "   Peak Velocity [mm/s]  Vel. Cal. Uncert.  Peak Uncert." << endl;

  for (Int_t i=0; i<6; i++) {

    cout << "Peak " << i << ": " << peakVel[i] << " +- " << peakVelVelUnc[i] << " +- " << peakVelPosUnc[i] << endl;

    peakVelUnc.push_back(TMath::Sqrt(peakVelVelUnc[i]*peakVelVelUnc[i]+peakVelPosUnc[i]*peakVelPosUnc[i]));

  }

  vector<Double_t> peakEn;
  vector<Double_t> peakEnUnc;

  cout << " dE " << endl;

  for (Int_t i=0; i<6; i++) {
    peakEn.push_back(nomE*peakVel[i]/cLight);
    peakEnUnc.push_back(nomE*peakVelUnc[i]/cLight);
    cout << "Peak " << i << ": " << nomE*peakVel[i]/cLight << " +- " << nomE*peakVelUnc[i]/cLight << endl;
  }

  vector<Double_t> g1;
  vector<Double_t> dg1;

  g1.push_back(peakEn[0]-peakEn[1]);
  g1.push_back(peakEn[1]-peakEn[2]);
  g1.push_back(peakEn[4]-peakEn[3]);
  g1.push_back(peakEn[5]-peakEn[4]);

  dg1.push_back(TMath::Sqrt(peakEnUnc[0]*peakEnUnc[0]+peakEnUnc[1]*peakEnUnc[1]));
  dg1.push_back(TMath::Sqrt(peakEnUnc[1]*peakEnUnc[1]+peakEnUnc[2]*peakEnUnc[2]));
  dg1.push_back(TMath::Sqrt(peakEnUnc[4]*peakEnUnc[4]+peakEnUnc[3]*peakEnUnc[3]));
  dg1.push_back(TMath::Sqrt(peakEnUnc[5]*peakEnUnc[5]+peakEnUnc[4]*peakEnUnc[4]));

  vector<Double_t> g0;
  vector<Double_t> dg0;

  g0.push_back(peakEn[3]+peakEn[1]);
  g0.push_back(peakEn[4]+peakEn[2]);
  dg0.push_back(TMath::Sqrt(peakEnUnc[3]*peakEnUnc[3]+peakEnUnc[1]*peakEnUnc[1]));
  dg0.push_back(TMath::Sqrt(peakEnUnc[4]*peakEnUnc[4]+peakEnUnc[2]*peakEnUnc[2]));

  Double_t avg_g1=0;
  Double_t std_g1=0;

  Double_t avg_g0=0;
  Double_t std_g0=0;

  for (Int_t i=0; i<4; i++) {
    avg_g1+=g1[i]/(dg1[i]*dg1[i]);
    std_g1+=1/(dg1[i]*dg1[i]);
  }

  avg_g1=avg_g1/std_g1;
  std_g1=TMath::Sqrt(1/std_g1);

  for (Int_t i=0; i<2; i++) {
    avg_g0+=g0[i]/(dg0[i]*dg0[i]);
    std_g0+=1/(dg0[i]*dg0[i]);
  }

  avg_g0=avg_g0/std_g0;
  std_g0=TMath::Sqrt(1/std_g0);

  Double_t mu_rat = -3*avg_g1/avg_g0;
  Double_t mu_rat_unc = mu_rat*TMath::Sqrt((std_g0*std_g0)/(avg_g0*avg_g0)+(std_g1*std_g1)/(avg_g1*avg_g1));
  Double_t mu0 = 0.0903;
  Double_t mu0_unc = 0.0007;

  cout << "g1      = " << avg_g1 << " +- " << std_g1 << endl;
  cout << "g0      = " << avg_g0 << " +- " << std_g0 << endl;
  cout << "mu1/mu0 = " << mu_rat << " +- " << mu_rat_unc << endl;
  cout << "mu0     = " << mu0 << " +- " << mu0_unc << "*** not our measurement***" << endl;
  cout << "mu1     = " << mu_rat*mu0 << " +- " << mu_rat*mu0*TMath::Sqrt(mu_rat_unc*mu_rat_unc/(mu_rat*mu_rat)+mu0_unc*mu0_unc/(mu0*mu0)) << endl;

}
