//----------------------------------------------------------------------------
//
// Velocity curve derivation from second set of interferometric data
// Also determination of the energy splittings in 57Fe and magnetic moments
// of first and second excited states
// (3/12/14)
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

TF1 *fitParts1, *fitParts2;

Double_t diff(double *x, double *par) {
  return TMath::Abs(fitParts1->EvalPar(x,par) - fitParts2->EvalPar(x,par));
}

void vel_cal_2() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);
  TGaxis::SetMaxDigits(4);

  Int_t upLim = 2048;
  Int_t loLim = 200;

  Int_t nPasses = 2323; //from abs_cal_trial2
  Float_t dwell = 0.1; //ms

  Float_t lambda = 0.6328; //micron

  Double_t nomE=14.3e3; //eV
  Double_t cLight=3e11; //mm/s

  TH1D *inf = new TH1D("inf", "inf", 2048, 0, 2048);  
  TH1D *fe = new TH1D("fe", "fe", 2048, 0, 2048);

  confParse("data_list.txt", 3, inf);
  confParse("data_list.txt", 4, fe);

  //TF1 *blah = new TF1("blah", "[0]", 0, 2048);
  //blah->SetParameter(0,1);

  //inf->Multiply(blah, lambda/(2*nPasses*dwell));

  inf->SetLineColor(kRed);
  inf->SetTitle("");
  inf->GetXaxis()->SetTitle("MCA Bin");
  inf->GetXaxis()->SetNdivisions(8,5,0);
  inf->GetYaxis()->SetTitle("Velocity [mm/s]");

  fe->SetTitle("");
  fe->GetXaxis()->SetTitle("MCA Bin");
  fe->GetXaxis()->SetNdivisions(8,5,0);
  fe->GetYaxis()->SetTitle("Counts");

  inf->Draw();

  // fit to interferometric stuff
  TF1 *fitNom = new TF1("fitNom", "[0]+abs([1]*(x-[2])+[3]*(x-[2])**2)", loLim, upLim);
  TF1 *fitExt = new TF1("fitExt", "[0]+abs([1]*(x-[2])+[3]*(x-[2])**2)", 100, upLim);

  fitParts1 = new TF1("fitParts1", "[0]+[1]*x", 500, 1040);
  fitParts2 = new TF1("fitParts2", "[0]+[1]*x", 1080, 1600);

  fitNom->SetParameter(0, 250);
  fitNom->SetParameter(1, 10);
  fitNom->SetParameter(2, 1060);

  fitExt->SetParameter(0, 250);
  fitExt->SetParameter(1, 10);
  fitExt->SetParameter(2, 1060);

  TF1 *fitPeak = new TF1("fitPeak", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)-[13]*([15]/2)^2/((x-[14])^2+([15]/2)^2)-[16]*([18]/2)^2/((x-[17])^2+([18]/2)^2)-[19]*(x-[20])**2", loLim, upLim);

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

  fitParts1->SetLineColor(kGreen);
  fitParts2->SetLineColor(kGreen);

  TGraph *residual1 = new TGraph();
  TGraph *residual2 = new TGraph();
  TGraph *residual3 = new TGraph();

  TF1 *test = new TF1("test", "fitNom-fitExt", loLim, upLim);

  inf->Fit("fitNom", "RN");
  inf->Fit("fitExt", "RN");
  inf->Fit("fitParts1", "RN");
  inf->Fit("fitParts2", "RN");

  TF1 *diffPart = new TF1("diffPart", diff, loLim, upLim, 0);
  Double_t b = diffPart->GetMinimumX();

  for (Int_t i=0; i<(upLim-loLim); i++) {
    residual1->SetPoint(i, loLim+i, (fitNom->Eval(loLim+i)-inf->GetBinContent(loLim+i))/inf->GetBinContent(loLim+i));
    //residual1->SetPoint(i, loLim+i, (fitNom->Eval(loLim+i)-inf->GetBinContent(loLim+i)));
    residual2->SetPoint(i, loLim+i, (fitExt->Eval(loLim+i)-inf->GetBinContent(loLim+i))/fitExt->Eval(loLim+i));
    if ((loLim+i)<b) {
      residual3->SetPoint(i, loLim+i, (fitParts1->Eval(loLim+i)-inf->GetBinContent(loLim+i))/inf->GetBinContent(loLim+i));
      //residual3->SetPoint(i, loLim+i, (fitParts1->Eval(loLim+i)-inf->GetBinContent(loLim+i)));
    }
    else {
      residual3->SetPoint(i, loLim+i, (fitParts2->Eval(loLim+i)-inf->GetBinContent(loLim+i))/inf->GetBinContent(loLim+i));
      //residual3->SetPoint(i, loLim+i, (fitParts2->Eval(loLim+i)-inf->GetBinContent(loLim+i)));
    }
  }

  fitNom->Draw("same");
  /*
  TCanvas *c2 = MakeCanvas("c2", "c2", 600, 600);
  c2->Divide(1,2);
  c2->cd(1);
  //residual1->GetYaxis()->SetRangeUser(-1.5,0.5);
  residual1->Draw("al");
  cout << residual1->GetMean(2) << endl;
  c2->cd(2);
  //residual2->GetYaxis()->SetRangeUser(-1.5,0.5);
  //residual2->Draw("al");
  //c2->cd(3);
  //residual3->GetYaxis()->SetRangeUser(-1.5,0.5);
  residual3->Draw("al");
  cout << residual3->GetMean(2) << endl;
  */
  
  c1->SaveAs("inter.png");

  cout << "From choice of end point" << endl;
  cout << (fitNom->Eval(200)-fitExt->Eval(200))/fitNom->Eval(200) << endl;
  cout << (fitNom->Eval(1000)-fitExt->Eval(1000))/fitNom->Eval(1000) << endl;
  cout << (fitNom->Eval(2000)-fitExt->Eval(2000))/fitNom->Eval(2000) << endl;
  cout << "from abs vs pieces" << endl;
  cout << (fitNom->Eval(200)-fitParts1->Eval(200))/fitNom->Eval(200) << endl;
  cout << (fitNom->Eval(1000)-fitParts1->Eval(1000))/fitNom->Eval(1000) << endl;
  cout << (fitNom->Eval(1100)-fitParts2->Eval(1100))/fitNom->Eval(1100) << endl;
  cout << (fitNom->Eval(2000)-fitParts2->Eval(2000))/fitNom->Eval(2000) << endl;

  fe->SetLineColor(kRed);
  fe->Draw("");
  fitPeak->Draw("same");
  fe->Fit("fitPeak", "RN");

  c1->SaveAs("fePeaks.png");

  b = fitNom->GetParameter(1);
  Double_t c = fitNom->GetParameter(2);
  Double_t d = fitNom->GetParameter(3);

  TF1 *velNom = new TF1("velNom", "abs([0]*(x-[1])+[2]*(x-[1])**2)", loLim, upLim);
  velNom->SetParameter(0, b*lambda/(2*nPasses*dwell));
  velNom->SetParameter(1, c);
  velNom->SetParameter(2, d*lambda/(2*nPasses*dwell));

  b = fitExt->GetParameter(1);
  c = fitExt->GetParameter(2);
  d = fitExt->GetParameter(3);

  TF1 *velExt = new TF1("velNom", "abs([0]*(x-[1])+[2]*(x-[1])**2)", loLim, upLim);
  velExt->SetParameter(0, b*lambda/(2*nPasses*dwell));
  velExt->SetParameter(1, c);
  velExt->SetParameter(2, d*lambda/(2*nPasses*dwell));

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

  peakPos.push_back(fitPeak->GetParameter(2));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(3))));
  peakPos.push_back(fitPeak->GetParameter(5));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(6))));
  peakPos.push_back(fitPeak->GetParameter(8));  peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(9))));
  peakPos.push_back(fitPeak->GetParameter(11)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(12))));
  peakPos.push_back(fitPeak->GetParameter(14)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(15))));
  peakPos.push_back(fitPeak->GetParameter(17)); peakPosUnc.push_back(TMath::Sqrt(fabs(fitPeak->GetParameter(18))));

  vector<Double_t> peakVel;
  vector<Double_t> peakVelUnc;
  vector<Double_t> peakVelVelUnc1;
  vector<Double_t> peakVelVelUnc2;
  vector<Double_t> peakVelPosUnc;

  cout << "    Peak Channel   Uncert" << endl;
  for (Int_t i=0; i<6; i++) {
    cout << "Peak " << i << ": " << peakPos[i] << " +- " << peakPosUnc[i] << endl;

    peakVel.push_back(velNom->Eval(peakPos[i]));
    if (i<3) {
      peakVelVelUnc1.push_back(fabs(velNom->Eval(peakPos[i])-velPart1->Eval(peakPos[i])));
    }
    else {      
      cout << velNom->Eval(peakPos[i]) << " " << velPart2->Eval(peakPos[i]) << endl;
      peakVelVelUnc1.push_back(fabs(velNom->Eval(peakPos[i])-velPart2->Eval(peakPos[i]))); 
    }
    peakVelVelUnc2.push_back(fabs(velNom->Eval(peakPos[i])-velExt->Eval(peakPos[i])));
    peakVelPosUnc.push_back(TMath::Max(fabs(velNom->Eval(peakPos[i])-velNom->Eval(peakPos[i]+peakPosUnc[i])), fabs(velNom->Eval(peakPos[i])-velNom->Eval(peakPos[i]-peakPosUnc[i]))));

  }
  cout << endl;

  cout << "   Peak Velocity [mm/s]  Vel. Cal. Uncerts.  Peak Uncert." << endl;

  for (Int_t i=0; i<6; i++) {

    peakVelUnc.push_back(TMath::Sqrt(peakVelVelUnc1[i]*peakVelVelUnc1[i]+peakVelPosUnc[i]*peakVelPosUnc[i]+peakVelVelUnc2[i]*peakVelVelUnc2[i]));
    cout <<  peakVel[i] << " \\pm " << peakVelVelUnc1[i] << " \\pm " << peakVelVelUnc2[i] << " \\pm " << peakVelPosUnc[i] << endl; //"( " << peakVelUnc[i] << " )" << endl;
    //cout <<   peakVel[i] << " \\pm " << 100*peakVelVelUnc1[i]/peakVel[i] << "% +- " << 100*peakVelVelUnc2[i]/peakVel[i] << "% +- " << 100*peakVelPosUnc[i]/peakVel[i] << "% ( " << 100*peakVelUnc[i]/peakVel[i] << "% )" << endl;

  }

  vector<Double_t> peakEn;
  vector<Double_t> peakEnUnc;
  Double_t cog=0;
  Double_t cogunc=0;

  cout << " dE " << endl;

  for (Int_t i=0; i<6; i++) {
    peakEn.push_back(nomE*peakVel[i]/cLight);
    peakEnUnc.push_back(nomE*peakVelUnc[i]/cLight);
    cout << "Peak " << i << ": " << nomE*peakVel[i]/cLight << " +- " << nomE*peakVelUnc[i]/cLight << endl;
    if (i<3) cog-=nomE*peakVel[i]/cLight;
    else cog+=nomE*peakVel[i]/cLight;
    cogunc+=(nomE*peakVelUnc[i]/cLight)*(nomE*peakVelUnc[i]/cLight);
  }

  cout << "Center of gravity: " << cog/6 << "\\pm" << TMath::Sqrt(cogunc) << endl;

  vector<Double_t> g1;
  vector<Double_t> dg1;

  Double_t h0= peakEn[5]+peakEn[0];
  Double_t h0unc= TMath::Sqrt(peakEn[5]*peakEn[5]+peakEn[0]*peakEn[0]);

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

  cout << "hin     = " << h0 << " +- " << h0unc << endl;
  cout << "g1      = " << avg_g1 << " +- " << std_g1 << endl;
  cout << "g0      = " << avg_g0 << " +- " << std_g0 << endl;
  cout << "mu1/mu0 = " << mu_rat << " +- " << mu_rat_unc << endl;
  cout << "mu0     = " << mu0 << " +- " << mu0_unc << "*** not our measurement***" << endl;
  cout << "mu1     = " << mu_rat*mu0 << " +- " << mu_rat*mu0*TMath::Sqrt(mu_rat_unc*mu_rat_unc/(mu_rat*mu_rat)+mu0_unc*mu0_unc/(mu0*mu0)) << endl;

}
