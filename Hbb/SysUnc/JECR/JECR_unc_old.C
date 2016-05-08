#include <iostream>
#include <fstream>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TLegend.h"

using namespace std;

int JECR_unc(int type_int, int type_jec){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	

//	int type_int; //double  =0; single=1
	
	TString type;
	if (type_int==0) type = "_double";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "DoubleB";
	if (type_int==1) text_type = "SingleB";
	TString jec;
	if (type_jec==0) jec="JER";
	if (type_jec==1) jec = "JEC";

	const int num_ss = 2;	

	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg","GF"};
//	TString s_names[num_ss] = {"vbf_76"};
	TString tex_s_names[num_ss] = {"VBF, m(H) = 125 GeV", "GF, m(H) = 125 GeV"};

	TFile *file_s1 =  new TFile("../../output_hist/v14/golden/skimmed_tree_analysis"+type+s_names[0]+"_v14_final_76jec.root");
	TH1F *hist_bdt_JEnom = (TH1F*)file_s1->Get("hbdt_"+jec+"nom");
	TH1F *hist_bdt_JEup= (TH1F*)file_s1->Get("hbdt_"+jec+"up");
	TH1F *hist_bdt_JEdown = (TH1F*)file_s1->Get("hbdt_"+jec+"down");
	
	hist_bdt_JEnom->Scale(2190.);
	hist_bdt_JEup->Scale(2190.);
	hist_bdt_JEdown->Scale(2190.);
	hist_bdt_JEnom->Sumw2(kFALSE);
	hist_bdt_JEup->Sumw2(kFALSE);
	hist_bdt_JEdown->Sumw2(kFALSE);




	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.15);
	c1->SetRightMargin(.25);
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,-1.,1.);
	frame2->SetMinimum(0.);
   frame2->SetMaximum(hist_bdt_JEnom->GetMaximum()*1.6);
//	if (type_int==1)  frame2->SetMaximum(0.005);
   frame2->SetStats(0);
	frame2->SetYTitle("Events / cat");
	frame2->SetXTitle("BDT output");	
	frame2->GetYaxis()->SetNdivisions(505);
	frame2->GetXaxis()->SetLabelSize(0.0);
  	frame2->GetXaxis()->SetTitleSize(0.05);
  	frame2->GetXaxis()->SetLabelSize(0.04);
	frame2->Draw();
	TLatex* tex = new TLatex(0.75,0.95,"13 TeV");
   tex->SetNDC();
	tex->SetTextAlign(35);
   tex->SetTextFont(42);
   tex->SetTextSize(0.035);
   tex->SetLineWidth(2);
   TLatex *tex1 = new TLatex(0.17,0.95,"CMS");
   tex1->SetNDC();
   tex1->SetTextAlign(20);
   tex1->SetTextFont(61);
   tex1->SetTextSize(0.04);
   tex1->SetLineWidth(2);
   TLatex* tex2 = new TLatex(0.25,0.89,"Work in progress");
   tex2->SetNDC();
   tex2->SetTextAlign(20);
   tex2->SetTextFont(52);
   tex2->SetTextSize(0.035);
  	tex2->SetLineWidth(2);	
	TLatex* tex_file = new TLatex(0.36,0.95,text_type);
   tex_file->SetNDC();
	tex_file->SetTextAlign(35);
   tex_file->SetTextFont(42);
   tex_file->SetTextSize(0.04);
   tex_file->SetLineWidth(2);	
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
//	tex_file->Draw();
	hist_bdt_JEnom->SetLineColor(4);
	hist_bdt_JEnom->SetLineWidth(2);
	hist_bdt_JEup->SetLineColor(2);
	hist_bdt_JEup->SetLineWidth(2);
	hist_bdt_JEdown->SetLineColor(8);
	hist_bdt_JEdown->SetLineWidth(2);
	hist_bdt_JEnom->Draw("same");
	hist_bdt_JEup->Draw("same");
	hist_bdt_JEdown->Draw("same");

////////

	TLegend *leg = new TLegend(0.77,0.6,0.92,0.9);
	leg->SetFillColor(0);
//	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(0.025);
	leg->SetHeader(jec+" variation");
	leg->AddEntry(hist_bdt_JEnom,"central","L");
	leg->AddEntry(hist_bdt_JEdown,jec+" down","L");
	leg->AddEntry(hist_bdt_JEup,jec+" up","L");
	leg->Draw("same");
	c1->Print("plots/v14_vbf_"+type+"_"+jec+".png");
		


	return 0;
}

