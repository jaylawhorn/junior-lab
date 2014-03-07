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

void cp_test(TString conf="cp_water_70") {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);

  TGaxis::SetMaxDigits(3);

  vector<TString> fnamev;
  vector<Double_t> timev;
  vector<Double_t> maxv;
  vector<TGraphErrors*> graphv;

  confParse(conf+TString(".txt"), fnamev, timev, graphv);

  Int_t i=2;

  carrPurcell(fnamev[i], i, conf, graphv[i]);

  Double_t tx=0, ty=0;
  Double_t tx1=0, ty1=0;

  for (Int_t j=0; j<graphv[i]->GetN(); j++) {
    
      graphv[i]->GetPoint(j, tx, ty);
      graphv[i]->GetPoint(j-1, tx1, ty1);

      if ( fabs(tx-0.00652-(0.004*int((tx-0.00652)/0.004))) < 0.001 ) {
	graphv[i]->RemovePoint(j-2);
	graphv[i]->RemovePoint(j-2);
	graphv[i]->RemovePoint(j-2);
	graphv[i]->RemovePoint(j-2);

	}
  }
  
  TGraphErrors *extract = new TGraphErrors();
  Double_t max_x=-1, max_y=-1;
  Double_t min_x=1, min_y=1;
  Int_t k=0;

  for (Int_t j=0; j<graphv[i]->GetN(); j++) {

    graphv[i]->GetPoint(j, tx, ty);
    graphv[i]->GetPoint(j+1, tx1, ty1);

    if (tx<0.003) continue;

    //if (fabs(tx1-tx) >0.001) {
    if (fabs(tx1-tx) >0.0005) {
      if (max_y==-1) continue;
      cout << "hi" << endl;
      extract->SetPoint(k, 0.5*1000*(max_x+min_x), 0.5*1000*(max_y-min_y));
      extract->SetPointError(k, 0, TMath::Sqrt(0.5*(2*0.4*0.4)));
      k++;

      max_x=-1; max_y=-1;
      min_x=1; min_y=1;
    }
    else if (ty>max_y) {
      max_y=ty;
      max_x=tx;
    }
    else if (ty<min_y) {
      min_y=ty;
      min_x=tx;
    }
     
  }

  //graphv[i]->Draw("ap");
  extract->SetMarkerColor(kRed);
  extract->SetTitle("");
  extract->GetXaxis()->SetTitle("Tau [ms]");
  extract->GetYaxis()->SetTitle("Spin Echo Height [mV]");
  extract->Draw("ap");

  TF1 *fitfxn = new TF1("fitfxn", "expo", 5, 45);
  extract->Fit("fitfxn", "R");

  Float_t t1 = -1.0/fitfxn->GetParameter(1);
  Double_t uncert = max(fabs(t1+(1.0/(fitfxn->GetParameter(1)+fitfxn->GetParError(1)))), fabs(t1+(1.0/(fitfxn->GetParameter(1)-fitfxn->GetParError(1)))));

  cout << fabs(t1+(1.0/(fitfxn->GetParameter(1)+fitfxn->GetParError(1)))) << ", " << fabs(t1+(1.0/(fitfxn->GetParameter(1)-fitfxn->GetParError(1)))) << endl;
  cout << "T1 = " << -1.0/fitfxn->GetParameter(1) << " +- " << uncert << endl;
  
}
