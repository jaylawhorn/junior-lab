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

void cp_test(TString conf="cp_water_30") {

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

    if (fabs(tx1-tx) >0.001) {

      extract->SetPoint(k, 0.5*(max_x+min_x), 0.5*(max_y-min_y));
      //extract->SetPointError(k, TMath::Sqrt(0.5*(min_x*max_x+min_x*max_x)-0.25*(max_x+min_x)*(max_x+min_x)), TMath::Sqrt(0.5*(2*0.4e-3*0.4e-3)-0.4e-3));
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

  graphv[i]->Draw("ap");
  extract->SetMarkerColor(kRed);
  extract->Draw("same p");

}
