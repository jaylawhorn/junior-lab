#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TGaxis.h>
#include <TMath.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TBox.h>
#include <TF1.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "rbfxns.hh"
#include "plotStyle.hh"
#endif

Float_t b(Float_t turns, Float_t current, Float_t diameter);

Float_t getLowBar(TGraph *gr, Float_t upper, Float_t xmin, Float_t xmax);

Float_t getUppBar(TGraph *gr, Float_t upper, Float_t xmin, Float_t xmax);

Float_t getMin(TGraph *grB, TGraph *grO, Float_t xmin, Float_t xmax, Float_t freq);

void new_f(TString filename="data/1501.csv", Float_t freq=150) {

  Float_t iZmin=35.276;
  Float_t zTurns=180;
  Float_t zDia=22.5;
  Float_t ohm=49.8;

  Float_t bZ = b(zTurns, iZmin, zDia);
  //cout << bZ*1e7 << endl;
  //bZ=TMath::Sqrt(bZ*bZ+b(50, 20, 22.5)*b(50, 20,22.5));

  TGraph *gr1 = new TGraph(filename, "%lg %lg" , ",");
  TGraph *gr2 = new TGraph(filename, "%lg %*lg %lg" , ",");
  TGraph *gr3 = new TGraph();
  TGraph *gr4 = new TGraph();

  Double_t x, y;

  for(Int_t i=0; i<gr1->GetN(); i++) {
    gr1->GetPoint(i,x,y);
    gr1->SetPoint(i,x,y*1e3);
  }

  Float_t temp=0;
  for (Int_t i=0; i<gr2->GetN(); i++) {
    gr2->GetPoint(i, x, y);
    //temp=fabs(b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4);
    //temp=TMath::Sqrt(( b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4 )*( b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4 )+b(50, 40, 22.5)*b(50, 40,22.5)*1e8);
    temp=TMath::Sqrt(( b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4 )*( b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4) +0.08*0.08);
    //cout << b(50, 40, 22.5)*1e4 << " " << ( b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4 ) << " " << temp -fabs( b(zTurns, y*1e3/ohm, zDia)*1e4-bZ*1e4 )  <<  endl;
    gr3->SetPoint(i, x, temp);
    //gr4->SetPoint(i, x, fabs(b(zTurns, y*1e3/ohm, zDia)*1e5-bZ*1e5)-3.5);
  }
  /*
  gr1->Draw();
  gr1->GetXaxis()->SetRangeUser(-3,1);
  gr4->SetMarkerColor(kRed);
  gr4->SetLineColor(kRed);
  gr4->Draw("same l");

  TGaxis *axis = new TGaxis(gPad->GetUxmax(),gr1->GetYaxis()->GetXmin(),
			    gPad->GetUxmax(),gr1->GetYaxis()->GetXmax(),
			    gr1->GetYaxis()->GetXmin()*0.1+0.35,gr1->GetYaxis()->GetXmax()*0.1+0.35,505,"+L");
  axis->SetLabelFont(42);
  axis->SetLabelSize(0.050);
  axis->SetTitleFont(42);
  axis->SetTitleSize(0.055);
  axis->SetTitleOffset(1.200);
  axis->SetLabelOffset(0.010);
  axis->SetLabelColor(kRed);
  axis->SetTitleColor(kRed);
  axis->SetTitle("B Field [G]");
  axis->Draw();
  */
  if (freq==100) {
    getMin(gr3,gr1,-0.20,0,freq);
    getMin(gr3,gr1,0.40,0.70,freq);
    getMin(gr3,gr1,1.0,1.20,freq);
    getMin(gr3,gr1,1.7,1.9,freq);
  }
  if (freq==110) {                                                                         
    //getMin(gr3,gr1,-1.4,-1.3,freq);                                                                 
    getMin(gr3,gr1,-0.4,-0.2,freq);                                                                 
    getMin(gr3,gr1,0.4,0.5,freq);                                                                   
    getMin(gr3,gr1,1.1,1.3,freq);                                                                   
    getMin(gr3,gr1,1.9,2.1,freq);                                                                   
    //getMin(gr3,gr1,3.7,3.9,freq);                                                                   
  }   
  if (freq==120) {                                                                             
    //getMin(gr3,gr1,-2.0,-1.9,freq);                                                                 
    getMin(gr3,gr1,-1.2,-1.1,freq);                                                                 
    getMin(gr3,gr1,-0.6,-0.4,freq);                                                                 
    getMin(gr3,gr1,0.2,0.4,freq);                                                                   
    getMin(gr3,gr1,1.3,1.4,freq);                                                                   
    //getMin(gr3,gr1,2.1,2.3,freq);                                                                   
    //getMin(gr3,gr1,2.8,2.9,freq);                                                                   
    //getMin(gr3,gr1,3.6,3.7,freq);                                                                   
  }                                                                                            
  if (freq==130) {                                                                             
    getMin(gr3,gr1,-1.6,-1.5,freq);
    getMin(gr3,gr1,-1.0,-0.8,freq);                                                                 
    getMin(gr3,gr1,0.2,0.3,freq);                                                                   
    getMin(gr3,gr1,2.2,2.3,freq);                                                                   
    //getMin(gr3,gr1,3.3,3.5,freq);                                                                   
  } 
  if (freq==140) {                                                                             
    //getMin(gr3,gr1,-4.9,-4.8,freq);                                                                 
    getMin(gr3,gr1,-4.3,-4.1,freq);                                                                 
    getMin(gr3,gr1,-3.4,-3.2,freq);                                                                 
    getMin(gr3,gr1,-0.9,-0.7,freq);                                                                 
    getMin(gr3,gr1,0.1,0.3,freq);                                                                   
    //getMin(gr3,gr1,0.7,0.9,freq);                                                                   
    //getMin(gr3,gr1,1.7,1.9,freq);                                                                   
    //getMin(gr3,gr1,4.1,4.3,freq);                                                                   
    //getMin(gr3,gr1,5.1,5.3,freq);                                                                   
    //getMin(gr3,gr1,5.7,5.9,freq);                                                                   
    //getMin(gr3,gr1,6.7,6.9,freq);                                                                   
  }                                                                                            
  if (freq==141) {                                                                             
    //getMin(gr3,gr1,-5.0,-4.8,freq);                                                                 
    getMin(gr3,gr1,-2.6,-2.4,freq);                                                                 
    getMin(gr3,gr1,-1.7,-1.4,freq);                                                                 
    getMin(gr3,gr1,-1.0,-0.8,freq);                                                                 
    getMin(gr3,gr1,0.0,0.2,freq);                                                                   
    //getMin(gr3,gr1,2.3,2.7,freq);                                                                   
    //getMin(gr3,gr1,3.3,3.7,freq);                                                                   
    //getMin(gr3,gr1,4.0,4.3,freq);                                                                   
    //getMin(gr3,gr1,4.9,5.2,freq);                                                                   
  }                                                                                            
  if (freq==150) {                                                                             
    getMin(gr3,gr1,-2.5,-2,freq);                                                                   
    getMin(gr3,gr1,-1.5,-1.3,freq);                                                                 
    getMin(gr3,gr1,-1.0,-0.5,freq);                                                                 
    getMin(gr3,gr1,-0.5,0.1,freq);                                                                  
    //getMin(gr3,gr1,2.6,2.7,freq);                                                                   
  }                           

}

Float_t b(Float_t turns, Float_t current, Float_t diameter) {
  return 8*4e-7*TMath::Pi()*turns*current/1000/(TMath::Sqrt(125)*diameter/2*0.0254);
}

Float_t getLowBar(TGraph *gr, Float_t upper, Float_t xmin, Float_t xmax) {
  Double_t x, y;
  for (Int_t i=0; i<gr->GetN(); i++) {
    gr->GetPoint(i, x, y);
    if (x<xmin) continue;
    if (x>xmax) continue;
    if (y<upper) return x;
  }
  return 0;
}

Float_t getUppBar(TGraph *gr, Float_t upper, Float_t xmin, Float_t xmax) {
  Double_t x, y;
  for (Int_t i=gr->GetN(); i>-1; i--) {
    gr->GetPoint(i, x, y);
    if (x<xmin) continue;
    if (x>xmax) continue;
    if (y<upper) return x;
  }
  return 0;
}

Float_t getMin(TGraph *grB, TGraph *grO, Float_t xmin, Float_t xmax, Float_t freq) {

  // Find minimum value

  Float_t min=1000000;
  Float_t xSave=0;
  Float_t xSave1=0;
  Float_t xSave2=0;
  Double_t x, y;

  for (Int_t i=0; i<grO->GetN(); i++) {
    grO->GetPoint(i, x, y);
    if (x<xmin) continue;
    if (x>xmax) continue;
    if (y<min) { min=y; xSave1=x; }
  }

  min=1000000;

  for (Int_t i=grO->GetN(); i>-1; i--) {
    grO->GetPoint(i, x, y);
    if (x<xmin) continue;
    if (x>xmax) continue;
    if (y<min) { min=y; xSave2=x; }
  }
  //cout << xSave1 << " " << xSave2 << endl;
  xSave=0.5*(xSave1+xSave2);

  // Find statistical scatter in optical signal

  Int_t iMax=100;
  Float_t mean=0;
  float_t stdev=0;
  for (Int_t i=0; i<iMax; i++) {
    grO->GetPoint(i, x, y);
    mean+=y;
    stdev+=y*y;
  }
  mean=mean/iMax;
  stdev=stdev/iMax;
  stdev=TMath::Sqrt(stdev-mean*mean);
  Float_t upper=grO->Eval(xSave)+3*stdev;
  Float_t upErr = getUppBar(grO, upper, xmin, xmax);
  Float_t loErr = getLowBar(grO, upper, xmin, xmax);

  TF1* fitfxn = new TF1("fitfxn", "[0]+[1]*x", xmin, xmax);

  grB->Fit("fitfxn", "RQ");

  Float_t temp = TMath::Max(fabs(fitfxn->Eval(upErr)-fitfxn->Eval(xSave)),fabs(fitfxn->Eval(xSave)-fitfxn->Eval(loErr)));
  Float_t temp2 = fitfxn->Eval(xSave)*(4.71/417.75);
  //cout << temp*100  << ", " << temp2*100 << endl;
  temp = TMath::Sqrt(temp*temp+temp2*temp2);

  cout << freq << " " << fitfxn->Eval(xSave) << " " << temp << endl;
  /*
  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);
  
  grO->SetMarkerSize(0.5);
  grO->SetLineColor(0);
  grO->SetTitle("");
  grO->GetXaxis()->SetTitle("Time [ms]");
  grO->GetXaxis()->SetRangeUser(-2.5,-2.2);
  grO->GetYaxis()->SetTitle("Voltage [mV]");
  grO->Draw();

  TLine *line = new TLine(-2.5, upper, -2.2, upper);
  line->SetLineColor(kRed);
  line->SetLineWidth(3);

  TLine *line2 = new TLine(xSave, grO->GetYaxis()->GetXmin(), xSave, grO->GetYaxis()->GetXmax());
  line2->SetLineColor(kRed);
  line2->SetLineWidth(3);

  TBox *box = new TBox(upErr, grO->GetYaxis()->GetXmin(), loErr, grO->GetYaxis()->GetXmax());
  box->SetFillColor(kRed-9);
  //box->Draw();
  //line2->Draw();
  line->Draw();
  grO->Draw("same p");
  */
}
