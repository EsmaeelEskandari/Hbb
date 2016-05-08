#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"

//int main(){
void mean(){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	const int n_variables = 7 ;//10;// 11; // 18;//19;
	TString variables_names[n_variables]={"CAT0","CAT1","CAT2","CAT3","CAT4","CAT5","CAT6"};  
	Double_t frame2_axisx[n_variables];
	
	Double_t fwhm[n_variables]={29.73,28.54,27.48,24.87,30.11,28.65,27.03};
	Double_t sigma[n_variables]={12.4,12.0,11.6,10.5,12.6,12.1,11.4};
	Double_t mean[n_variables]={122.6,122.3,122.2,123.6,123.1,123.1,123.6};
	Double_t mean_mh[n_variables];

	for (int i=0;i<n_variables;i++){
		fwhm[i]/=mean[i];
		sigma[i]/=mean[i];
		mean[i]/=125.;
	}

	for (int i=0;i<n_variables;i++){
		frame2_axisx[i] = 0.5+i;
	}



		TCanvas *c2 = new TCanvas("c2","",800,800);
		c2->cd();
		TH1F *frame2 = new TH1F("frame2","",n_variables,0.,n_variables);
      frame2->SetStats(0);
		frame2->GetXaxis()->SetNdivisions(505);
   	frame2->SetStats(0);
	//	frame2->SetYTitle("FWHM / m");
	//	frame2->GetYaxis()->SetRangeUser(0.15,0.3);
	//	frame2->SetYTitle("#sigma / m");
	//	frame2->GetYaxis()->SetRangeUser(0.05,0.14);
		frame2->SetYTitle("m / M_{H}");
		frame2->GetYaxis()->SetNdivisions(104);
		frame2->GetYaxis()->SetRangeUser(0.97,1.0);
		frame2->SetXTitle("");	
		for (int i=0;i<n_variables;i++){
			frame2->GetXaxis()->SetBinLabel(i+1,variables_names[i]);
		}
		frame2->Draw();
	//	TGraph *gr = new TGraph(n_variables,frame2_axisx,fwhm);
	//	TGraph *gr = new TGraph(n_variables,frame2_axisx,sigma);
		TGraph *gr = new TGraph(n_variables,frame2_axisx,mean);
		gr->SetMarkerStyle(20);
		gr->SetLineWidth(2);
		gr->Draw("PLsame");
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
	pCMS2.Draw("same");
	gPad->Update();
	pave.Draw("same");
	pCMS1.Draw("same");
	pCMS12.Draw("same");
	pCMS2.Draw("same");



		c2->Print("plots/mean.pdf");


}
