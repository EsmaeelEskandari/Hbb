#include <iostream>
#include <fstream>
#include <algorithm>
#include <TH1F.h>
#include <TF1.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "THStack.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TLegend.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooCBShape.h"
#include "RooChebychev.h"
#include "RooBernstein.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "TCanvas.h"
#include "RooAbsPdf.h"
#include "RooFit.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
#include "RooPlot.h"
#include "TPaveText.h"
#include "TVirtualPad.h"


int main(){
//void bias(){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	const int n_variables = 7 ;//10;// 11; // 18;//19;
	TString variables_names[n_variables]={"CAT0","CAT1","CAT2","CAT3","CAT4","CAT5","CAT6"}; 

		TFile *f_run1 = new TFile("/afs/cern.ch/work/n/nchernya/Hbb/Combine/toolkit/flat/fitforflat/Fit_data_selVBF.root"); 
		f_run1->cd("Hbb");
		TTree *t_run1 = (TTree*) f_run1->Get("Hbb/events");
		TString cut_run1 = "mvaVBF<=-0.6";  //NOM -0.1(0.4), -0.6 (0.0)
//		TString cut_run1 = "mvaNOM<=-0.1";  //NOM -0.1(0.4), -0.6 (0.0)
//		TString cut_run1 = "mvaVBF>-0.6&&mvaVBF<=0.0";  //NOM -0.1(0.4), -0.6 (0.0)
//		TString cut_run1 = "mvaNOM>-0.1&&mvaNOM<0.4";  //NOM -0.1(0.4), -0.6 (0.0)
		TH1F *hist_run1 = new TH1F("hist_run1","hist_run1",60,80,200);
		hist_run1->Sumw2(kTRUE);
		t_run1->Draw("mbbReg>>hist_run1",cut_run1);
		hist_run1->Scale(1./hist_run1->Integral());
		hist_run1->SetStats(kFALSE);			
		hist_run1->SetLineColor(kRed);			
		hist_run1->SetMarkerColor(kRed);			
		hist_run1->SetMarkerStyle(20);			
		hist_run1->SetLineWidth(2);	
//		hist_run1->Fit("pol4","N");	
		TF1 *func_run1 = new TF1("func_run1","pol4",80,200);	
//		TF1 *func_run1 = new TF1("func_run1","pol5",0.,300);	
		func_run1->SetLineColor(kRed);
		func_run1->SetParameters(0.09,0.002,3.3e-05,2.8e-07,9.8e-10);
//		func_run1->SetParameters(0.02,-0.00022,-24.e-07,5.4e-09,1.e-13);
//		func_run1->SetParLimits(0,-1.,1.);
//		func_run1->SetParLimits(1,-0.1,0.1);
//		func_run1->SetParLimits(1,-0.01,0.);
//		func_run1->SetParLimits(2,-1.e-02,1.e-02);
//		func_run1->SetParLimits(3,-1.e-04,1.e-04);
//		func_run1->SetParLimits(4,-1.e-05,1.e-05);
		hist_run1->Fit(func_run1);


/*
//////////////////////Roo Fit////////////////////
//RooRealVar brn_run1[10];
RooRealVar brn_run1;
TString brn_name;
RooArgList brn_params_run1;
//for (int i=0;i<5;i++){
//	brn_run1[i] = RooRealVar(brn_name.Format("run1_brn_%d",i).Data(),brn_name.Format("run1_brn_%d",i).Data(),0.5,0.,10.);
	brn_run1 = RooRealVar("run1_brn","run1_brn",0.5,0.,10.);
	brn_params_run1.add(brn_run1);
//	brn_run1[i].setConstant(kFALSE);
	brn_run1.setConstant(kFALSE);
//}
RooRealVar	x("mbbReg","mbbReg",80,200);
TH1F* hist_fit_run1 = (TH1F*)hist_run1->Clone("hist_run1_fit");
RooDataHist rh("mbbReg_hist","mbbReg_hist",RooArgList(x),hist_fit_run1);
RooRealVar yQ("yield_QCD","yield_QCD",0.5,0.,1.);
RooBernstein qcd_pdf_aux("qcd_model_aux","qcd_model_aux",x,brn_params_run1);
RooAddPdf model("model","model",RooArgList(qcd_pdf_aux),RooArgList(yQ));
model.fitTo(rh);

*/


/////////////////////////////////////////////////


		
	
		TFile *f_run2 = new TFile("/afs/cern.ch/work/n/nchernya/Hbb/Combine/toolkit/flat/fitforflat/Fit_BTagCSV_selDouble.root"); 
		f_run2->cd("Hbb");
		TTree *t_run2 = (TTree*) f_run2->Get("Hbb/events");
		TString cut_run2 = "mvaDouble<=0.36";  //single 0.28(72), 0.36(76)
//		TString cut_run2 = "mvaSingle<=0.28";  //single 0.28(72), 0.36(76)
//		TString cut_run2 = "mvaDouble>0.36&&mvaDouble<=0.76";  //single 0.28(72), 0.36(76)
//		TString cut_run2 = "mvaSingle>0.28&&mvaSingle<=0.72";  //single 0.28(72), 0.36(76)
		TH1F *hist_run2 = new TH1F("hist_run2","",60,80,200);
		hist_run2->Sumw2(kTRUE);
		t_run2->Draw("mbbRegFSR>>hist_run2",cut_run2);
		hist_run2->Scale(1./hist_run2->Integral());
		hist_run2->SetStats(kFALSE);			
		hist_run2->SetLineColor(kBlue+1);			
		hist_run2->SetMarkerColor(kBlue+1);			
		hist_run2->SetMarkerStyle(21);			
		hist_run2->SetLineWidth(2);			
	//	hist_run2->Fit("pol4","N");		
//		hist_run2->Fit("pol5","N");		/		TF1 *func_run2 = new TF1("func_run2","pol4",80,200);	
//		TF1 *func_run2 = new TF1("func_run2","pol5",80,200);	
		TF1 *func_run2 = new TF1("func_run2","pol4",0.,300);
		func_run2->SetParameters(0.09,0.002,3.3e-05,2.8e-07,9.8e-10);
//		func_run2->SetParameters(0.015,-0.00036,-38.e-07,9.e-09,1.e-13);
//		func_run2->SetParLimits(0,-1.,1.);
	//	func_run2->SetParLimits(1,-0.01,-1.e-07);
//		func_run2->SetParLimits(2,-1.e-02,1.e-02);
//		func_run2->SetParLimits(3,-1.e-04,1.e-04);
//		func_run2->SetParLimits(4,-1.e-05,1.e-05);
		func_run2->SetLineColor(kBlue+1);	
		hist_run2->Fit(func_run2);





		

		TCanvas *c2 = new TCanvas("c2","",750,800);
		c2->cd();
		gPad->SetLogy(0);
		c2->SetBottomMargin(.3);
		c2->SetLeftMargin(.17);
		TH1F *frame = new TH1F("frame","",60,80.,200);
		frame->SetMaximum(0.03);
      frame->SetStats(0);
		frame->GetXaxis()->SetNdivisions(505);
	 	frame->GetXaxis()->SetLabelSize(0.0);
   	frame->SetStats(0);
		frame->SetYTitle("PDF");
	//	frame->GetYaxis()->SetNdivisions(104);
		frame->GetYaxis()->SetTitleOffset(1.5);
		frame->Draw();

		hist_run1->Draw("PEsame");
		hist_run2->Draw("PEsame");
	TLegend *leg = new TLegend(0.65,0.76,0.9,0.9);
	leg->SetFillColor(0);
	leg->SetFillStyle(-1);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(0.03);
//	leg->AddEntry(hist_run1,"Run I, NOM","P");
//	leg->AddEntry(hist_run2,"Run II, SingleB","P");
	leg->AddEntry(hist_run1,"Run I, PRK","P");
	leg->AddEntry(hist_run2,"Run II, DoubleB","P");
	leg->Draw("same");
  		gPad->RedrawAxis();

float right   = gStyle->GetPadRightMargin();
float top = gStyle->GetPadTopMargin();
float left =  gStyle->GetPadLeftMargin();
float bottom = gStyle->GetPadBottomMargin();


	TPaveText pave(0.6,0.8,1.-right-top*0.5333,1.-top*1.666,"NDC");
	pave.SetTextAlign(11);
	pave.SetFillStyle(-1);
	pave.SetBorderSize(0);
	pave.SetTextFont(42);
	pave.SetTextSize(top*0.5);
	pave.SetTextColor(kBlue+1);
	pave.Draw("same");


// CMS info
	float left2 = gStyle->GetPadLeftMargin();
	float right2 = gStyle->GetPadRightMargin();
	float top2 = gStyle->GetPadTopMargin();
	float bottom2 = gStyle->GetPadBottomMargin();




	TPaveText pCMS1(left2,1.-top2,0.4,1.,"NDC");
	pCMS1.SetTextFont(62);
	pCMS1.SetTextSize(top2*0.75);
	pCMS1.SetTextAlign(12);
	pCMS1.SetFillStyle(-1);
	pCMS1.SetBorderSize(0);
	pCMS1.AddText("CMS");
	TPaveText pCMS12(left2+0.1,1.-top2*1.1,0.6,1.,"NDC");
	pCMS12.SetTextFont(52);
	pCMS12.SetTextSize(top2*0.75);
	pCMS12.SetTextAlign(12);
	pCMS12.SetFillStyle(-1);
	pCMS12.SetBorderSize(0);
	pCMS12.AddText("Preliminary");
	TPaveText pCMS2(0.5,1.-top2,1.-right2*0.5,1.,"NDC");
	pCMS2.SetTextFont(42);
	pCMS2.SetTextSize(top2*0.75);
	pCMS2.SetTextAlign(32);
	pCMS2.SetFillStyle(-1);
	pCMS2.SetBorderSize(0);
	pCMS2.AddText("L = 2.32 fb^{-1} (13 TeV)");
	gPad->Update();
	pCMS1.Draw("same");
//	pCMS2.Draw("same");
	gPad->Update();
	pave.Draw("same");
	pCMS1.Draw("same");
//	pCMS12.Draw("same");

  		TPad* pad2 = new TPad("pad2", "pad2", 0., 0., 1., 1.);
 		pad2->SetTopMargin(0.73);
		pad2->SetLeftMargin(.17);
 		pad2->SetFillColor(0);
  		pad2->SetFillStyle(0);
  		pad2->Draw();
  		pad2->cd(0);
  		gPad->SetGridy();

		TH1F *frame2 = new TH1F("frame2","",1,80.,200);
		frame2->SetMinimum(0.5);
      frame2->SetMaximum(1.5);
      frame2->SetStats(0);
      frame2->SetTitleFont(42,"x");
		frame2->SetTitleFont(42,"y");
      frame2->SetTitleSize(0.13, "XYZ");
		frame2->GetYaxis()->SetNdivisions(505);
 		frame2->GetYaxis()->SetTickLength(0.06);
  		frame2->GetYaxis()->SetTitleSize(0.04);
  		frame2->GetYaxis()->SetTitleOffset(1.5);
 		frame2->GetYaxis()->SetLabelSize(0.03);
  		frame2->GetYaxis()->CenterTitle(kTRUE);
  		frame2->GetXaxis()->SetTitleSize(0.05);
  		frame2->GetXaxis()->SetLabelSize(0.04);
		frame2->SetXTitle("m_{bb} (GeV)");

		frame2->Draw();
 		Double_t aa[2] = {80,200};
   	Double_t bb[2] = {1,1};
   	TGraph *cons = new TGraph(2,aa,bb);
    	cons->SetLineStyle(2);
		cons->Draw("Lsame");
		TH1F *hist_run1_clone = (TH1F*)hist_run1->Clone("new_run1");
		hist_run1_clone->SetMarkerColor(kBlack);
		hist_run1_clone->SetLineColor(kBlack);
		hist_run1_clone->Divide(hist_run2);
		hist_run1_clone->Draw("PEsame");
		c2->Print("plots/double_cat.pdf");


}
