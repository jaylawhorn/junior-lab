//----------------------------------------------------------------------------
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

void b_min() {

  TCanvas *c1 = MakeCanvas("c1", "c1", 800, 600);
  TGaxis::SetMaxDigits(4);


  Float_t Xcurrent1[12] = { 80, 90, 100, 109.9, 119.9, // mA, +- 0.2
			    130, 140, 150, 160, 169.9, 
			    180.4, 188.5 };

  Float_t Xcurrent2[11] = { 130, 133, 136, 139, 142, 
			    145, 148, 151, 154, 157, 
			    160 };

  Float_t Xcurrent3[22] = { 135, 137, 139, 141, 143, 
			    145, 147, 149, 151, 153, 
			    155, 157, 159, 161, 163, 
			    165, 130, 125, 120, 170, 
			    175, 180 };

  Float_t Xtime1[12] = { 252, 220, 188, 164, 144, // ms, +- 2
			 128, 124, 120, 128, 136, 
			 156, 172 };
  Float_t Xtime2[11] = { 232, 228, 224,
			 220, 216, 216, 216, 216, 
			 220, 224, 228 };

  Float_t Xtime3[22] = { 202, 196, 190, 186, 186, 
			 184, 184, 184, 184, 186,
			 186, 190, 194, 198, 202,
			 208, 218, 236, 264, 228,
			 252, 284 };

  Float_t Xtime4[22] = { 914, 906, 900, 894, 890,
			 888, 886, 888, 888, 890,
			 894, 894, 900, 908, 914,
			 924, 938, 970, 1004, 956,
			 992, 1018 };

  Float_t Xlo1 = 80; // kHz
  Float_t Xlo2 = 90; // kHz
  Float_t Xhi1 = 180; // kHz
  Float_t Xhi2 = 130; // kHz
  Float_t Xhi3 = 160; // kHz
  Float_t sweepT1 = 500; // ms
  Float_t sweepT2 = 1000; // ms

  Float_t Xfreq=0;
  Float_t dXfreq=0;
  Float_t dXcurrent=0.2;

  TGraphErrors *grX1 = new TGraphErrors(12);
  TGraphErrors *grX2 = new TGraphErrors(11);
  TGraphErrors *grX3 = new TGraphErrors(22);
  TGraphErrors *grX4 = new TGraphErrors(22);

  Float_t off=200;

  for (Int_t i=0; i<12; i++) {

    Xfreq = Xlo1 + (Xtime1[i]+off)*(Xhi1-Xlo1)/sweepT1;
    dXfreq = 2*(Xhi1-Xlo1)/sweepT1;

    grX1->SetPoint(i, Xcurrent1[i], Xfreq);
    grX1->SetPointError(i, dXcurrent, dXfreq);

  }

  for (Int_t i=0; i<11; i++) {
   
    Xfreq = Xlo1 + (Xtime2[i]+off)*(Xhi2-Xlo1)/sweepT1;
    dXfreq = 2*(Xhi2-Xlo1)/sweepT1;

    grX2->SetPoint(i, Xcurrent2[i], Xfreq);
    grX2->SetPointError(i, dXcurrent, dXfreq);

  }  

  for (Int_t i=0; i<22; i++) {
   
    Xfreq = Xlo2 + (Xtime3[i]+off)*(Xhi3-Xlo2)/sweepT2;
    dXfreq = 2*(Xhi3-Xlo2)/sweepT2;

    grX3->SetPoint(i, Xcurrent3[i], Xfreq);
    grX3->SetPointError(i, dXcurrent, dXfreq);

  }  

  for (Int_t i=0; i<22; i++) {
   
    Xfreq = Xlo2 + (Xtime4[i]+off)*(Xhi3-Xlo2)/sweepT2;
    dXfreq = 2*(Xhi3-Xlo2)/sweepT2;

    grX4->SetPoint(i, Xcurrent3[i], Xfreq);
    grX4->SetPointError(i, dXcurrent, dXfreq);

  }  
  grX1->SetTitle("");
  grX1->GetXaxis()->SetTitle("I_{x} [mA]");
  grX1->GetYaxis()->SetTitle("Absorption Peak [kHz]");

  grX1->Draw("ap");
  grX2->Draw("same p");
  //grX3->Draw("same p");
  //grX4->Draw("same p");

  TF1 *par1 = new TF1("par1", "[0]+[1]*x+[2]*x^2", 75, 200);
  TF1 *par2 = new TF1("par2", "[0]+[1]*x+[2]*x^2", 125, 165);
  TF1 *par3 = new TF1("par3", "[0]+[1]*x+[2]*x^2", 120, 180);
  TF1 *par4 = new TF1("par4", "[0]+[1]*x+[2]*x^2", 120, 180);
  //TF1 *par1 = new TF1("par1", "[0]+[1]*(x-[2])^2", 75, 200);
  //TF1 *par2 = new TF1("par2", "[0]+[1]*(x-[2])^2", 125, 165);
  //TF1 *par3 = new TF1("par3", "[0]+[1]*(x-[2])^2", 120, 180);
  //TF1 *par4 = new TF1("par4", "[0]+[1]*(x-[2])^2", 120, 180);
  par1->SetLineColor(kRed);
  par2->SetLineColor(kRed);
  par3->SetLineColor(kRed);
  par4->SetLineColor(kRed);

  grX2->Fit("par2", "REM");

  Float_t a=par2->GetMinimumX(125, 165);
  Float_t da=0;
  da = (par2->GetParError(1)*par2->GetParameter(1))*(par2->GetParError(1)*par2->GetParameter(1));
  da+= (par2->GetParError(2)*par2->GetParameter(2))*(par2->GetParError(2)*par2->GetParameter(2));
  da= TMath::Sqrt(da);
  //da=par2->GetParError(2);

  grX1->Fit("par1", "R");

  Float_t b=par1->GetMinimumX(75, 200);
  Float_t db=0;
  db = (par1->GetParError(1)*par1->GetParameter(1))*(par1->GetParError(1)*par1->GetParameter(1));
  db+= (par1->GetParError(2)*par1->GetParameter(2))*(par1->GetParError(2)*par1->GetParameter(2));
  db= TMath::Sqrt(db);
  //db=par1->GetParError(2);

  grX3->Fit("par3", "REM");

  Float_t f=par3->GetMinimumX(120, 180);
  Float_t df=0;
  df = (par3->GetParError(1)*par3->GetParameter(1))*(par3->GetParError(1)*par3->GetParameter(1));
  df+= (par3->GetParError(2)*par3->GetParameter(2))*(par3->GetParError(2)*par3->GetParameter(2));
  df= TMath::Sqrt(df);
  //df=par3->GetParError(2);

  grX4->Fit("par4", "REM");

  Float_t g=par4->GetMinimumX(120, 180);
  Float_t dg=0;
  dg = (par4->GetParError(1)*par4->GetParameter(1))*(par4->GetParError(1)*par4->GetParameter(1));
  dg+= (par4->GetParError(2)*par4->GetParameter(2))*(par4->GetParError(2)*par4->GetParameter(2));
  dg= TMath::Sqrt(dg);
  //dg=par4->GetParError(2);

  c1->SaveAs("x_min.png");

  cout << a << ", " << b << ", " << f << ", " << g << endl;
  cout << da << ", " << db << ", " << df << ", " << dg << endl;

  Float_t temp=(a/(da*da)+b/(db*db)+f/(df*df)+g/(dg*dg))/(1/(da*da)+1/(db*db)+1/(df*df)+1/(dg*dg));
  Float_t temp2 = TMath::Sqrt(1/(1/(da*da)+1/(db*db)+1/(df*df)+1/(dg*dg)));

  cout << "Bx_min = " << temp << " +- " << temp2 << endl;
  
  Float_t Zcurrent[10] = { 10, 15, 20, 25, 30, 
			   40, 45, 50, 55, 60 };

  Float_t Zcurrent1[12] = { 0, 8.9, 10, 15, 20,
			    25, 30, 40, 45, 50,
			    55, 60 };

  Float_t Zt1[10] = { 704, 600, 496, 408, 344, 
		      344, 400, 488, 584, 688 };

  Float_t Zt2[10] = { 480, 408, 344, 288, 240, 
		      240, 280, 336, 400, 472 };

  Float_t Zt3[12] = { 548, 428, 414, 350, 288, 
		      236, 198, 198, 232, 290, 
		      354, 418 };

  Float_t Zt4[12] = { 786, 610, 592, 498, 412,
		      334, 278, 274, 336, 416, 
		      504, 596 };

  Float_t Zlo = 0.001; // kHz
  Float_t Zhi = 170; // kHz
  Float_t Zlo2 = 0; // kHz
  Float_t Zhi2 = 200; // kHz
  Float_t sweepT = 1000; // ms

  Float_t Zfreq1=0;
  Float_t Zfreq2=0;
  Float_t dZfreq=0;
  Float_t dZcurrent=0.2;

  TGraphErrors *grZ1 = new TGraphErrors(10);
  TGraphErrors *grZ2 = new TGraphErrors(10);

  for (Int_t i=0; i<10; i++) {

    Zfreq1 = Zlo + (Zt1[i]+off)*(Zhi-Zlo)/sweepT;
    Zfreq2 = Zlo + (Zt2[i]+off)*(Zhi-Zlo)/sweepT;
    dZfreq = 8*(Zhi-Zlo)/sweepT;
    if ((Zcurrent[i]==25)||(Zcurrent[i]==30)||(Zcurrent[i]==40)) dZfreq*=2;
    
    grZ1->SetPoint(i, Zcurrent[i], Zfreq1);
    grZ1->SetPointError(i, dZcurrent, dZfreq);
    grZ2->SetPoint(i, Zcurrent[i], Zfreq2);
    grZ2->SetPointError(i, dZcurrent, dZfreq);

  }

  TGraphErrors *grZ3 = new TGraphErrors(12);
  TGraphErrors *grZ4 = new TGraphErrors(12);

  for (Int_t i=0; i<12; i++) {

    Zfreq1 = Zlo2 + (Zt3[i]+off)*(Zhi2-Zlo2)/sweepT;
    Zfreq2 = Zlo2 + (Zt4[i]+off)*(Zhi2-Zlo2)/sweepT;
    dZfreq = 8*(Zhi2-Zlo2)/sweepT;
    if ((Zcurrent1[i]==25)||(Zcurrent1[i]==30)||(Zcurrent1[i]==40)) dZfreq*=2;
    
    grZ3->SetPoint(i, Zcurrent1[i], Zfreq1);
    grZ3->SetPointError(i, dZcurrent, dZfreq);
    grZ4->SetPoint(i, Zcurrent1[i], Zfreq2);
    grZ4->SetPointError(i, dZcurrent, dZfreq);

  }

  grZ1->SetTitle("");
  grZ1->GetXaxis()->SetTitle("I_{z} [mA]");
  grZ1->GetYaxis()->SetTitle("Absorption Peak [kHz]");
  grZ1->GetYaxis()->SetRangeUser(30, 130);

  grZ1->Draw("ap");
  grZ2->Draw("same p");
  grZ3->Draw("same p");
  grZ4->Draw("same p");

  TF1 *par5 = new TF1("par5", "[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4", 0, 63);
  TF1 *par6 = new TF1("par6", "[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4", 0, 63);
  TF1 *par7 = new TF1("par7", "[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4", 0, 63);
  TF1 *par8 = new TF1("par8", "[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4", -3, 63);
  par8->SetParameter(0, 165);
  par8->SetParameter(1, -6);
  par8->SetParameter(2, 0.086);

  grZ1->Fit("par5", "REM");
  grZ2->Fit("par6", "REM");
  grZ3->Fit("par7", "REM");
  grZ4->Fit("par8", "REM");

  c1->SaveAs("z_min4.png");

  cout << "test" << endl;

  a=par5->GetMinimumX(0, 63);
  da=0;
  da = (par5->GetParError(1)*par5->GetParameter(1))*(par5->GetParError(1)*par5->GetParameter(1));
  da+= (par5->GetParError(2)*par5->GetParameter(2))*(par5->GetParError(2)*par5->GetParameter(2));
  da+= (par5->GetParError(3)*par5->GetParameter(3))*(par5->GetParError(3)*par5->GetParameter(3));
  da+= (par5->GetParError(4)*par5->GetParameter(4))*(par5->GetParError(4)*par5->GetParameter(4));
  da= TMath::Sqrt(da);

  b=par6->GetMinimumX(0, 63);
  db=0;
  db = (par6->GetParError(1)*par6->GetParameter(1))*(par6->GetParError(1)*par6->GetParameter(1));
  db+= (par6->GetParError(2)*par6->GetParameter(2))*(par6->GetParError(2)*par6->GetParameter(2));
  db+= (par6->GetParError(3)*par6->GetParameter(3))*(par6->GetParError(3)*par6->GetParameter(3));
  db+= (par6->GetParError(4)*par6->GetParameter(4))*(par6->GetParError(4)*par6->GetParameter(4));
  db= TMath::Sqrt(db);

  f=par7->GetMinimumX(0, 63);
  df=0;
  df = (par7->GetParError(1)*par7->GetParameter(1))*(par7->GetParError(1)*par7->GetParameter(1));
  df+= (par7->GetParError(2)*par7->GetParameter(2))*(par7->GetParError(2)*par7->GetParameter(2));
  df+= (par7->GetParError(3)*par7->GetParameter(3))*(par7->GetParError(3)*par7->GetParameter(3));
  df+= (par7->GetParError(4)*par7->GetParameter(4))*(par7->GetParError(4)*par7->GetParameter(4));
  df= TMath::Sqrt(df);

  g=par8->GetMinimumX(0, 63);
  dg=0;
  dg = (par8->GetParError(1)*par8->GetParameter(1))*(par8->GetParError(1)*par8->GetParameter(1));
  dg+= (par8->GetParError(2)*par8->GetParameter(2))*(par8->GetParError(2)*par8->GetParameter(2));
  dg+= (par8->GetParError(3)*par8->GetParameter(3))*(par8->GetParError(3)*par8->GetParameter(3));
  dg+= (par8->GetParError(4)*par8->GetParameter(4))*(par8->GetParError(4)*par8->GetParameter(4));
  dg= TMath::Sqrt(dg);

  temp=(a/(da*da)+b/(db*db)+f/(df*df)+g/(dg*dg))/(1/(da*da)+1/(db*db)+1/(df*df)+1/(dg*dg));
  temp2 = TMath::Sqrt(1/(1/(da*da)+1/(db*db)+1/(df*df)+1/(dg*dg)));

  cout << a << ", " << b << ", " << f << ", " << g << endl;
  cout << da << ", " << db << ", " << df << ", " << dg << endl;

  cout << "Bz_min " <<  temp << " +- " << temp2 << endl;
  
}
