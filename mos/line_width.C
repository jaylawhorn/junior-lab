//----------------------------------------------------------------------------
//
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

void line_width() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);


    TF1 *fitPeak = new TF1("fitPeak", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*([6]/2)^2/((x-[5])^2+([6]/2)^2)-[7]*([9]/2)^2/((x-[8])^2+([9]/2)^2)-[10]*([12]/2)^2/((x-[11])^2+([12]/2)^2)-[13]*([15]/2)^2/((x-[14])^2+([15]/2)^2)-[16]*([18]/2)^2/((x-[17])^2+([18]/2)^2)-[19]*(x-[20])**2",200, 2040);

  char outfile[50];

  vector<Double_t> linewidths;
  vector<Double_t> linewidthsunc;
  vector<Double_t> err1;
  vector<Double_t> err2;
  for (Int_t i=21; i<27; i++) {

    TH1D *fecn = new TH1D("fecn", "fecn", 2048, 0, 2048);
    TH1D *cal = new TH1D("cal", "cal", 2048, 0, 2048);

    confParse("data_list.txt", i, fecn);
    if ((i==21)||(i==26)) confParse("data_list.txt", 4, cal);
    else confParse("data_list.txt", 30, cal);

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

    cal->SetTitle("");
    cal->GetXaxis()->SetTitle("MCA Channel");
    cal->GetYaxis()->SetTitle("Counts");
    cal->GetXaxis()->SetNdivisions(8,5,0);
    cal->SetLineColor(kRed);

    cal->Fit("fitPeak", "R");
    sprintf(outfile, "linewidthcal%i.png", i);

    c1->SaveAs(outfile);

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

    //TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)", 800, 1300);
    TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)", 900, 1200);
    //TF1 *fitPeak2 = new TF1("fitPeak2", "[0]-[1]*([3]/2)^2/((x-[2])^2+([3]/2)^2)-[4]*(x-[5])", 200, 2000);
    
    fitPeak2->SetParameter(0, 21500);
    fitPeak2->SetParameter(1, 2000);
    fitPeak2->SetParameter(2, 1040);
    fitPeak2->SetParameter(3, 50);

    fecn->SetTitle("");
    fecn->GetXaxis()->SetRangeUser(800, 1300);
    fecn->GetXaxis()->SetTitle("MCA Channel");
    fecn->GetYaxis()->SetTitle("Counts");
    fecn->GetXaxis()->SetNdivisions(8,5,0);
    fecn->SetLineColor(kRed);

    fecn->Draw();
    
    fecn->Fit("fitPeak2", "RN");
    fitPeak2->Draw("same");

    sprintf(outfile, "linewidth%i.png", i);

    c1->SaveAs(outfile);

    Double_t blah = fabs(velCurve->Eval(fitPeak2->GetParameter(2)+fitPeak2->GetParameter(3)/2)-velCurve->Eval(fitPeak2->GetParameter(2)-fitPeak2->GetParameter(3)/2));
    linewidths.push_back(fabs(velCurve->Eval(fitPeak2->GetParameter(2)+fitPeak2->GetParameter(3)/2)-velCurve->Eval(fitPeak2->GetParameter(2)-fitPeak2->GetParameter(3)/2)));
    Double_t temp1=fabs(velCurve->Eval(fitPeak2->GetParameter(2)+fitPeak2->GetParameter(3)/2-fitPeak2->GetParError(3)/2)-velCurve->Eval(fitPeak2->GetParameter(2)-fitPeak2->GetParameter(3)/2+fitPeak2->GetParError(3)/2));
    Double_t temp2=fabs(velCurve->Eval(fitPeak2->GetParameter(2)+fitPeak2->GetParameter(3)/2+fitPeak2->GetParError(3)/2)-velCurve->Eval(fitPeak2->GetParameter(2)-fitPeak2->GetParameter(3)/2-fitPeak2->GetParError(3)/2));
    Double_t moreblah = TMath::Max(fabs(temp1-blah), fabs(temp2-blah));
    Double_t andagain = fabs(getVelUnc(velCurve->Eval(fitPeak2->GetParameter(2)+fitPeak2->GetParameter(3)/2)));
    //Double_t andagain=0;
    err1.push_back(moreblah);
    err2.push_back(andagain);
    linewidthsunc.push_back(TMath::Sqrt(moreblah*moreblah+andagain*andagain));

    delete cal;
    delete fecn;
    peakPos.clear();
    peakPosUnc.clear();
  }

  TGraphErrors *gr = new TGraphErrors();

  for (Int_t i=0; i<linewidths.size(); i++) {

    cout << linewidths[i] << "\\pm" << err1[i] << "\\pm" << err2[i] << "\\pm" << linewidthsunc[i] << endl;
    gr->SetPoint(i, 25*(i+1), linewidths[i]);
    gr->SetPointError(i,0, linewidthsunc[i]);

  }

  gr->SetTitle("");
  gr->GetXaxis()->SetTitle("Absorber Thickness [mg/cm^{2}]");
  gr->GetYaxis()->SetTitle("Line Width [mm/s]");
  gr->Draw("ap");

  TF1 *thing = new TF1("thing", "[0]+[1]*x",0,7);

  gr->Fit("thing");

  Double_t nomE=14.4e3;
  Double_t cLight=3e11;
  cout << thing->GetParameter(0)*nomE/cLight << " " << thing->GetParError(0)*nomE/cLight << endl;

  c1->SaveAs("linewidth.png");

}
