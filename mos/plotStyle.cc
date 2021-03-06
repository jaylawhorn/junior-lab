#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TStyle.h>
#include "plotStyle.hh"

void plotStyle() {

  TStyle *jayStyle = new TStyle("jayStyle", "jayStyle");
  gStyle = jayStyle;

  jayStyle->SetCanvasColor(0);
  jayStyle->SetCanvasBorderSize(10);
  jayStyle->SetCanvasBorderMode(0);
  jayStyle->SetCanvasDefH(700);
  jayStyle->SetCanvasDefW(700);
  jayStyle->SetCanvasDefX(100);
  jayStyle->SetCanvasDefY(100);

  jayStyle->SetPadColor(0);
  jayStyle->SetPadBorderSize(10);
  jayStyle->SetPadBorderMode(0);
  jayStyle->SetPadBottomMargin(0.13);
  jayStyle->SetPadTopMargin(0.08);
  jayStyle->SetPadLeftMargin(0.15);
  jayStyle->SetPadRightMargin(0.05);
  jayStyle->SetPadGridX(0);
  jayStyle->SetPadGridY(0);
  jayStyle->SetPadTickX(0);
  jayStyle->SetPadTickY(0);
  
  jayStyle->SetHistFillColor(8);
  jayStyle->SetHistFillStyle(0);
  jayStyle->SetHistLineColor(1);
  jayStyle->SetHistLineStyle(0);
  jayStyle->SetHistLineWidth(2);

  jayStyle->SetFuncColor(1);
  jayStyle->SetFuncStyle(0);
  jayStyle->SetFuncWidth(2);

  jayStyle->SetMarkerStyle(20);
  jayStyle->SetMarkerSize(1);
  jayStyle->SetMarkerColor(1);

  jayStyle->SetOptStat(0);

  jayStyle->SetFrameFillStyle(0);
  jayStyle->SetFrameFillColor(0);
  jayStyle->SetFrameLineColor(1);
  jayStyle->SetFrameLineStyle(0);
  jayStyle->SetFrameLineWidth(1);
  jayStyle->SetFrameBorderSize(10);
  jayStyle->SetFrameBorderMode(0);

  jayStyle->SetStripDecimals(kFALSE);

  jayStyle->SetTitleBorderSize(0);
  jayStyle->SetTitleFillColor(0);
  jayStyle->SetTitleX(0.2);

  jayStyle->SetTitleSize(0.055, "X");
  jayStyle->SetTitleOffset(1.000,"X");
  jayStyle->SetLabelOffset(0.005,"X");
  jayStyle->SetLabelSize(0.050,"X");
  jayStyle->SetLabelFont(42,"X");

  jayStyle->SetTitleSize(0.055, "Y");
  jayStyle->SetTitleOffset(1.200,"Y");
  jayStyle->SetLabelOffset(0.010,"Y");
  jayStyle->SetLabelSize(0.050,"Y");
  jayStyle->SetLabelFont(42,"Y");

  jayStyle->SetTextSize(0.055);
  jayStyle->SetTextFont(42);
  jayStyle->SetTitleFont(42);
  jayStyle->SetTitleFont(42,"X");
  jayStyle->SetTitleFont(42,"Y");

  jayStyle->SetLegendBorderSize(10);
  jayStyle->SetLegendFillColor(0);
  jayStyle->SetLegendFont(42);

  return;

}

TCanvas* MakeCanvas(const char *name, const char *title, int dX, int dY) {

  TCanvas *canvas = new TCanvas(name, title, 0, 0, dX, dY);
  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetBorderSize(10);
  canvas->SetLeftMargin(0.18);
  canvas->SetRightMargin(0.05);
  canvas->SetTopMargin(0.08);
  canvas->SetBottomMargin(0.15);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameLineStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameLineStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);

  return canvas;

}
