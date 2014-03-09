#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TBox.h>
#include <TF1.h>
#include <TProfile.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "plotstyle.hh"
#include "nmrfxns.hh"
#endif

void viscosity(TString conf="g70_180t90") {

  TGaxis::SetMaxDigits(3);

  vector<TString> fnamev;
  vector<Double_t> timev;
  vector<Double_t> maxv;
  vector<TGraphErrors*> graphv;

  confParse(conf+TString(".txt"), fnamev, timev, graphv);

  Double_t echo_amp=0, d_echo_amp=0;
  Double_t echo_amp2=0, d_echo_choice=0;

  vector<Double_t> echo_ampv;
  vector<Double_t> echo_dampv;
  
  for (Int_t i=0; i<fnamev.size(); i++) {
    
    getPeaks(fnamev[i], timev[i], i, conf, graphv[i]);
  
    Double_t echo_max=0, echo_x=0;
    Double_t echo_max2=0, echo_x2=0;
    Double_t echo_min=0, echo_min_x=0;
    Double_t echo_min2=0, echo_min_x2=0;
    Double_t echo_max_dy=0, echo_min_dy=0;
    Double_t echo_max_dy2=0, echo_min_dy2=0;
    
    Double_t tx=0, ty=0;
    Double_t dy=0;
    
    for (Int_t j=0; j<graphv[i]->GetN(); j++) {
      
      graphv[i]->GetPoint(j, tx, ty);
      dy = graphv[i]->GetErrorY(j);

      if (ty>echo_max) { 
	echo_max2=echo_max;
	echo_x2 = echo_x;
	echo_max_dy2=echo_max_dy;
	echo_max=ty; 
	echo_x=tx; 
	echo_max_dy=dy;
      }
      else if (ty>echo_max2) {
	echo_max2=ty;
	echo_x2=tx;
	echo_max_dy2=dy;
      }
      if (ty<echo_min) {
	echo_min2=echo_min;
	echo_min_x2=echo_min_x;
	echo_min_dy2=echo_min_dy;
	echo_min=ty; 
	echo_min_x=tx; 
	echo_min_dy=dy;
      }
      else if (ty<echo_min2) {
	echo_min2=ty;
	echo_min_x2=tx;
	echo_min_dy2=dy;
      }
      
    }

    echo_amp = 0.5*(1000*echo_max-1000*echo_min);
    echo_amp2 = 0.5*(1000*echo_max2-1000*echo_min2);
    d_echo_amp = 1000*TMath::Sqrt(echo_max_dy*echo_max_dy+echo_min_dy*echo_min_dy);
    d_echo_choice = fabs(echo_amp-echo_amp2);

    cout << echo_amp << ", " << d_echo_amp << ", " << d_echo_choice << ", " << echo_x << ", " << echo_x2 << endl;

    //d_echo_amp = TMath::Sqrt(d_echo_amp*d_echo_amp+d_echo_choice*d_echo_choice);

    echo_ampv.push_back(echo_amp);
    echo_dampv.push_back(d_echo_amp);

    graphv[i]->Draw();

  }

  TCanvas *c2 = MakeCanvas("c2", "c2", 800, 600);

  Double_t avg=0;
  Double_t stdev=0;
  Double_t uncert=0;

  TGraphErrors *echo_height = new TGraphErrors(fnamev.size()/3);

  for (Int_t i=0; i<fnamev.size()/3; i++) {

    avg=(echo_ampv[3*i]+echo_ampv[3*i+1]+echo_ampv[3*i+2])/3.0;
    stdev=TMath::Sqrt((echo_ampv[3*i]*echo_ampv[3*i]+echo_ampv[3*i+1]*echo_ampv[3*i+1]+echo_ampv[3*i+2]*echo_ampv[3*i+2])/3-avg*avg);

    uncert=TMath::Sqrt(stdev*stdev+echo_dampv[3*i]*echo_dampv[3*i]+echo_dampv[3*i+1]*echo_dampv[3*i+1]+echo_dampv[3*i+2]*echo_dampv[3*i+2])/TMath::Sqrt(3);
    
    echo_height->SetPoint(i, timev[3*i], avg);
    echo_height->SetPointError(i, 0, uncert);

  }

  echo_height->SetTitle("");
  echo_height->GetXaxis()->SetTitle("Repeat Time [ms]");
  echo_height->GetYaxis()->SetTitle("FID Amplitude [mV]");

  echo_height->Draw("ap");

  TF1 *t1fit = new TF1("t1fit", "[0]-expo(1)", 0, 105);
  TF1 *t1fit2 = new TF1("t1fit2", "[0]-expo(1)", 10, 105);
  TF1 *t1fit3 = new TF1("t1fit3", "[0]-expo(1)", 20, 105);
  t1fit->SetLineColor(kBlue);

  cout << "t1" << endl;
  echo_height->Fit("t1fit", "MRN");
  echo_height->Fit("t1fit", "MRN");
  cout << "t12" << endl;
  echo_height->Fit("t1fit2", "MR");
  echo_height->Fit("t1fit2", "MR");
  cout << "t13" << endl;
  echo_height->Fit("t1fit3", "MRN");
  echo_height->Fit("t1fit3", "MRN");

  cout << endl;

  Float_t t1 = -1.0/t1fit->GetParameter(2);
  Float_t t12 = -1.0/t1fit2->GetParameter(2);
  Float_t t13 = -1.0/t1fit3->GetParameter(2);
  Double_t unc = fabs(t1fit->GetParError(2)/(t1fit->GetParameter(2)*t1fit->GetParameter(2)));
  Double_t unc2 = fabs(t1fit2->GetParError(2)/(t1fit2->GetParameter(2)*t1fit2->GetParameter(2)));
  Double_t unc3 = fabs(t1fit3->GetParError(2)/(t1fit3->GetParameter(2)*t1fit3->GetParameter(2)));

  cout << "T1  = " << t1 << " +- " << unc << endl;
  cout << "T12 = " << t12 << " +- " << unc2 << endl;
  cout << "T13 = " << t13 << " +- " << unc3 << endl;

}
