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

#endif
void testFit(const TString filename="data/Three-pulse measurements/t3_0.csv") {

  TGraph *gr = new TGraph(filename, "%lg %lg", ",");
  gr->Draw();


}
