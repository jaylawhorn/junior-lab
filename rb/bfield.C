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

#include "plotStyle.hh"
#endif

Float_t b(Float_t turns, Float_t current, Float_t diameter) {
    return 8*4e-7*TMath::Pi()*turns*current/1000/(TMath::Sqrt(125)*diameter/2*0.0254);
} // in Tesla

void bfield() {
  Float_t iXmin=148.32;
  //Float_t iXmin=146.411;
  Float_t iYmin=19.24;
  //Float_t iZmin=35.0;
  Float_t iZmin=35.276;
  Float_t dXmin=0.011;
  Float_t dYmin=3.0;
  Float_t dZmin=0.070;
  
  Float_t iX=145.9;
  Float_t iY=19.24;
  
  Float_t xTurns=50;
  Float_t xDia=14.3;
  Float_t yTurns=75;
  Float_t yDia=18;
  Float_t zTurns=180;
  Float_t zDia=22.5;
  
  Float_t ohm=49.8;
  
  Float_t bX = b(xTurns, iXmin, xDia);
  Float_t bY = b(yTurns, iYmin, yDia);
  Float_t bZ = b(zTurns, iZmin, zDia);

  Float_t dbX1 = TMath::Max(fabs(b(xTurns, iXmin+dXmin, xDia)-bX), fabs(b(xTurns, iXmin-dXmin, xDia)-bX));
  Float_t dbY1 = TMath::Max(fabs(b(yTurns, iYmin+dYmin, yDia)-bY), fabs(b(yTurns, iYmin-dYmin, yDia)-bY));
  Float_t dbZ1 = TMath::Max(fabs(b(zTurns, iZmin+dZmin, zDia)-bZ), fabs(b(zTurns, iZmin-dZmin, zDia)-bZ));

  Float_t dbX2 = TMath::Max(fabs(b(xTurns,iXmin,xDia-0.2)-bX),fabs(b(xTurns,iXmin,xDia+0.2)-bX));
  Float_t dbY2 = TMath::Max(fabs(b(yTurns,iYmin,yDia-0.2)-bY),fabs(b(yTurns,iYmin,yDia+0.2)-bY));
  Float_t dbZ2 = TMath::Max(fabs(b(zTurns, iZmin,zDia-0.2)-bZ),fabs(b(zTurns, iZmin,zDia+0.2)-bZ));

  //cout << "B_x = " << bX*1e7 << " +- " << TMath::Sqrt(dbX1*dbX1+dbX2*dbX2)*1e7 << " mG" << endl;
  //cout << "B_y = " << bY*1e7 << " +- " << TMath::Sqrt(dbY1*dbY1+dbY2*dbY2)*1e7 << " mG" << endl;
  //cout << "B_z = " << bZ*1e7 << " +- " << TMath::Sqrt(dbZ1*dbZ1+dbZ2*dbZ2)*1e7 << " mG" <<  endl;

  Float_t x=bX*1e7; Float_t dx=TMath::Sqrt(dbX1*dbX1+dbX2*dbX2)*1e7; 
  Float_t y=bY*1e7; Float_t dy=TMath::Sqrt(dbY1*dbY1+dbY2*dbY2)*1e7;
  Float_t z=bZ*1e7; Float_t dz=TMath::Sqrt(dbZ1*dbZ1+dbZ2*dbZ2)*1e7;

  Float_t dbX3 = fabs(380-x);
  Float_t dbY3 = fabs(70-y);
  Float_t dbZ3 = fabs(210-z);

  //dx = TMath::Sqrt(dx*dx+dbX3*dbX3);
  //dy = TMath::Sqrt(dy*dy+dbY3*dbY3);
  //dz = TMath::Sqrt(dz*dz+dbZ3*dbZ3);

  Float_t b = TMath::Sqrt(x*x+y*y+z*z);
  Float_t db = TMath::Sqrt(x*x*dx*dx+y*y*dy*dy+z*z*dz*dz)/b;

  cout << "B_x = " << x << " +- " << dx << " mG" << endl;
  cout << "B_y = " << y << " +- " << dy << " mG" << endl;
  cout << "B_z = " << z << " +- " << dz << " mG" << endl;
  cout << "|B| = " << b << " +- " << db << " mG" << endl;
  
}
