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
void magMoments() {

  Float_t pi = TMath::Pi();

  //***************************
  // Magnetometer calibration,
  // Magnetic field measurement
  //***************************

  Float_t calib = 202.5; // G
  Float_t meas  = 199.18; // G

  Float_t d_meas = (calib-meas)/calib; // % uncertainty from our calibration measurement

  Float_t d_calib = meas*TMath::Sqrt(d_meas*d_meas+0.01*0.01); // added in quadrature with calibration magnet uncertainty

  cout << d_calib/meas << endl;

  Float_t b_max = 1.762e3; // G
  Float_t db = 0.0005; // G
  Float_t db_tot = b_max*TMath::Sqrt((d_calib*d_calib)/(meas*meas)+db*db/(b_max*b_max)); // overall uncertainty on measured magnetic field

  cout << "Measured Magnetic field: " << endl;
  cout << b_max << " +- " << db_tot << " G" << endl;
  cout << endl;

  //b_max*=1e-4;
  //db_tot*=1e-4;

  //***************************
  // Hydrogen in glycerine
  //***************************

  Float_t f_max=7.5210; // MHz
  Float_t d_f = 0.0001; // MHz

  cout << "Measured Larmor frequency: " << endl;
  cout << f_max << " +- " << d_f << " MHz" << endl;
  cout << f_max*2*pi*1e6 << " +- " << 2*pi*d_f*1e6 << " rad/s" << endl;
  cout << endl;

  Float_t gamma = f_max*2*pi*1e6/b_max;
  Float_t dgamma = gamma*TMath::Sqrt((d_f*d_f)/(f_max*f_max)+(db_tot*db_tot)/(b_max*b_max));

  cout << "Measured gamma for H: " << endl;
  cout << gamma << " +- " << dgamma << " rad/ G s " << endl;
  cout << endl;

  //***************************
  // Fluorine in fluoropolymer
  //***************************

  f_max=7.076; // MHz
  d_f = 0.002; // MHz

  cout << "Measured Larmor frequency: " << endl;
  cout << f_max << " +- " << d_f << " MHz" << endl;
  cout << f_max*2*pi*1e6 << " +- " << 2*pi*d_f*1e6 << " rad/s" << endl;
  cout << endl;

  gamma = f_max*2*pi*1e6/b_max;
  dgamma = gamma*TMath::Sqrt((d_f*d_f)/(f_max*f_max)+(db_tot*db_tot)/(b_max*b_max));

  cout << "Measured gamma for F: " << endl;
  cout << gamma << " +- " << dgamma << " rad/ G s " << endl;
  cout << endl;

}
