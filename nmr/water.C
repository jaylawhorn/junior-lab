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

void water(TString conf="water_n2") {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  TGaxis::SetMaxDigits(3);

  vector<TString> fnamev;
  vector<Double_t> timev;
  vector<Double_t> maxv;
  vector<TGraphErrors*> graphv;

  confParse(conf+TString(".txt"), fnamev, timev, graphv);

  Double_t echo_amp=0, d_echo_amp=0;
  Double_t echo_amp2=0;

  Double_t avg_uncert=0;

  vector<Double_t> echo_ampv;
  vector<Double_t> echo_dampv;
  vector<Double_t> echo_sel_dampv;
  
  for (Int_t i=0; i<fnamev.size(); i++) {
    
    getPeaks(fnamev[i], timev[i], i, conf, graphv[i]);
  
    Double_t echo_max=0, echo_x=0;
    Double_t echo_min=0, echo_min_x=0;
    Double_t echo_max_dy=0, echo_min_dy=0;
    Double_t echo_max2=0, echo_x2=0;
    Double_t echo_min2=0, echo_min_x2=0;
    Double_t echo_max_dy2=0, echo_min_dy2=0;
    
    Double_t tx=0, ty=0;
    Double_t dy=0;
    
    for (Int_t j=0; j<graphv[i]->GetN(); j++) {
      
      graphv[i]->GetPoint(j, tx, ty);
      dy = graphv[i]->GetErrorY(j);

      if ((conf=="water_o2") && (i<3)) {
	if (tx<0.503) continue;
      }
      else if (tx<0.001) continue;
      
      if (ty>echo_max) { 
	echo_max2=echo_max;
	echo_x2=echo_x;
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
    
    echo_amp = 0.5*(echo_max-echo_min);
    echo_amp2 = 0.5*(echo_max2-echo_min2);
    d_echo_amp = TMath::Sqrt(echo_max_dy*echo_max_dy+echo_min_dy*echo_min_dy);

    echo_ampv.push_back(echo_amp);
    echo_dampv.push_back(d_echo_amp);
    echo_sel_dampv.push_back(fabs(echo_amp-echo_amp2));

    if (d_echo_amp<1e-3) avg_uncert+=d_echo_amp;

    cout << echo_amp << " " << echo_amp2 << " " << fabs(echo_amp-echo_amp2) << endl;

  }

  avg_uncert/=fnamev.size();

  Double_t avg=0;
  Double_t stdev=0;
  Double_t uncert=0;

  TGraphErrors *echo_height = new TGraphErrors(fnamev.size()/3);

  cout << endl;
  for (Int_t i=0; i<fnamev.size()/3; i++) {

    if (echo_dampv[3*i] > 1e-3) echo_dampv[3*i]=avg_uncert;
    if (echo_dampv[3*i+1] > 1e-3) echo_dampv[3*i+1]=avg_uncert;
    if (echo_dampv[3*i+2] > 1e-3) echo_dampv[3*i+2]=avg_uncert;

    avg=(echo_ampv[3*i]+echo_ampv[3*i+1]+echo_ampv[3*i+2])/3.0;
    stdev=TMath::Sqrt((echo_ampv[3*i]*echo_ampv[3*i]+echo_ampv[3*i+1]*echo_ampv[3*i+1]+echo_ampv[3*i+2]*echo_ampv[3*i+2])/3-avg*avg);
    cout << stdev << ",";
    uncert=stdev*stdev;
    if (uncert!=uncert) uncert=0;
    cout << uncert <<", ";
    uncert+=echo_dampv[3*i]*echo_dampv[3*i]+echo_dampv[3*i+1]*echo_dampv[3*i+1]+echo_dampv[3*i+2]*echo_dampv[3*i+2];
    cout << uncert <<", ";
    uncert+=echo_sel_dampv[3*i]*echo_sel_dampv[3*i]+echo_sel_dampv[3*i+1]*echo_sel_dampv[3*i+1]+echo_sel_dampv[3*i+2]*echo_sel_dampv[3*i+2];
    cout << uncert <<", ";
    uncert=TMath::Sqrt(uncert/3);

    cout << timev[3*i] << " " << 1000*avg << " 0 " << 1000*uncert << endl;
    
    echo_height->SetPoint(i, timev[3*i], 1000*avg);
    echo_height->SetPointError(i, 0, 1000*uncert);

  }
  cout << endl;

  echo_height->SetTitle("");
  echo_height->GetXaxis()->SetTitle("Repeat Time [s]");
  echo_height->GetYaxis()->SetTitle("Spin Echo Amplitude [mV]");
  echo_height->GetYaxis()->SetRangeUser(0, 20);

  TF1 *fitfxn = new TF1("fitfxn", "[0]-expo(1)", 0, 8.5);
  fitfxn->SetLineColor(kRed);

  echo_height->Draw("ap");

  echo_height->Fit("fitfxn");
  
  Double_t nom_t1 = -1.0/fitfxn->GetParameter(2);
  Double_t d_t1=fabs(fitfxn->GetParError(2)/(fitfxn->GetParameter(2)*fitfxn->GetParameter(2)));
  //Double_t d_t1 = TMath::Max(fabs(-1.0/(fitfxn->GetParameter(2)+fitfxn->GetParError(2))-nom_t1), fabs(-1.0/(fitfxn->GetParameter(2)-fitfxn->GetParError(2))-nom_t1));

  cout << "T1 = " << -1.0/fitfxn->GetParameter(2) << " +- " << d_t1 << endl;

}
