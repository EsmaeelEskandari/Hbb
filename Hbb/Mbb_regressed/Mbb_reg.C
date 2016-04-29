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

int Mbb_reg(int type_int){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	

//	int type_int; //double  =0; single=1
	
	TString type;
	if (type_int==0) type = "_double";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "DoubleB";
	if (type_int==1) text_type = "SingleB";

	const int num_ss = 1;	

//	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg"};
	TString s_names[num_ss] = {"vbf_76"};
	TString tex_s_names[num_ss] = {"VBF powheg, m(H) = 125 GeV"};

	TFile *file_s1 =  new TFile("../output_hist/v14/golden/skimmed_tree_analysis"+type+s_names[0]+"_v14_final_74cmssw_woweight.root");
	TH1F *hist_Mbb = (TH1F*)file_s1->Get("hMbb");
	TH1F *hist_Mbb_reg = (TH1F*)file_s1->Get("hMbb_regVBF");
	TH1F *hist_Mbb_reg_fsr = (TH1F*)file_s1->Get("hMbb_regVBF_fsr");
	TF1 *gaus = new TF1("gaus","gaus",95,140);
	gaus->SetLineColor(4);
	gaus->SetLineWidth(1);
	
	hist_Mbb->Sumw2(kFALSE);
	hist_Mbb_reg->Sumw2(kFALSE);
	hist_Mbb_reg_fsr->Sumw2(kFALSE);
	hist_Mbb->Sumw2(kFALSE);


	hist_Mbb->Fit(gaus,"R");
	Float_t Mbb_mean = gaus->GetParameter(1);
	Float_t Mbb_rms = gaus->GetParameter(2);
	hist_Mbb_reg->Sumw2(kFALSE);
	gaus->SetLineColor(2);
	hist_Mbb_reg->Fit(gaus,"R");
	Float_t Mbb_reg_mean = gaus->GetParameter(1);
	Float_t Mbb_reg_rms = gaus->GetParameter(2);
	hist_Mbb_reg_fsr->Sumw2(kFALSE);
	gaus->SetLineColor(8);
	hist_Mbb_reg_fsr->Fit(gaus,"R");
	Float_t Mbb_reg_fsr_mean = gaus->GetParameter(1);
	Float_t Mbb_reg_fsr_rms = gaus->GetParameter(2);

/////
	/*
	Float_t Mbb_mean = hist_Mbb->GetMean(1);
	Float_t Mbb_rms = hist_Mbb->GetRMS(1);
	Float_t Mbb_reg_mean = hist_Mbb_reg->GetMean(1);
	Float_t Mbb_reg_rms = hist_Mbb_reg->GetRMS(1);
	Float_t Mbb_reg_fsr_mean = hist_Mbb_reg_fsr->GetMean(1);
	Float_t Mbb_reg_fsr_rms = hist_Mbb_reg_fsr->GetRMS(1);
*/
/////



	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.15);
	c1->SetRightMargin(.25);
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,0.,230.);
	frame2->SetMinimum(0.);
   frame2->SetMaximum(hist_Mbb->GetMaximum()*1.6);
//	if (type_int==1)  frame2->SetMaximum(0.005);
   frame2->SetStats(0);
	frame2->SetYTitle("Events / 5 GeV");
	frame2->SetXTitle("M_{bb} (GeV)");	
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
	hist_Mbb->SetLineColor(4);
	hist_Mbb->SetLineWidth(2);
	hist_Mbb_reg->SetLineColor(2);
	hist_Mbb_reg->SetLineWidth(2);
	hist_Mbb_reg_fsr->SetLineColor(8);
	hist_Mbb_reg_fsr->SetLineWidth(2);
	hist_Mbb_reg->Draw("same");
	hist_Mbb_reg_fsr->Draw("same");
	hist_Mbb->Draw("same");

	TString leg_Mbb0 = "Nominal";
	TString leg_Mbb_reg0 = "Regressed";
	TString leg_Mbb_reg_fsr0 = "Regressed+FSR";
	TString leg_Mbb = "";
	TString leg_Mbb_reg = "";
	TString leg_Mbb_reg_fsr = "";
	TString leg_mean = "Mean=";
	TString leg_rms = ", RMS=";
	TString leg_mean = "#mu=";
	TString leg_rms = ", #sigma=";
	TString leg_ratio = ",#sigma/#mu=";
	TString temp_str;
	temp_str.Form("%2.1f",Mbb_mean);
	leg_Mbb.Append(leg_mean);
	leg_Mbb.Append(temp_str);
	leg_Mbb.Append(leg_rms);
	temp_str.Form("%2.1f",Mbb_rms);
	leg_Mbb.Append(temp_str);
	leg_Mbb.Append(leg_ratio);
	temp_str.Form("%2.2f",Mbb_rms/Mbb_mean);
	leg_Mbb.Append(temp_str);
///////
	temp_str.Form("%2.1f",Mbb_reg_mean);
	leg_Mbb_reg.Append(leg_mean);
	leg_Mbb_reg.Append(temp_str);
	leg_Mbb_reg.Append(leg_rms);
	temp_str.Form("%2.1f",Mbb_reg_rms);
	leg_Mbb_reg.Append(temp_str);
	leg_Mbb_reg.Append(leg_ratio);
	temp_str.Form("%2.2f",Mbb_reg_rms/Mbb_reg_mean);
	leg_Mbb_reg.Append(temp_str);
////////
	temp_str.Form("%2.1f",Mbb_reg_fsr_mean);
	leg_Mbb_reg_fsr.Append(leg_mean);
	leg_Mbb_reg_fsr.Append(temp_str);
	leg_Mbb_reg_fsr.Append(leg_rms);
	temp_str.Form("%2.1f",Mbb_reg_fsr_rms);
	leg_Mbb_reg_fsr.Append(temp_str);
	leg_Mbb_reg_fsr.Append(leg_ratio);
	temp_str.Form("%2.2f",Mbb_reg_fsr_rms/Mbb_reg_fsr_mean);
	leg_Mbb_reg_fsr.Append(temp_str);

	TLegend *leg = new TLegend(0.77,0.6,0.92,0.9);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(42);
	leg->SetTextSize(0.025);
	leg->AddEntry(hist_Mbb,text_type,"");
	leg->AddEntry(hist_Mbb,leg_Mbb0,"L");
	leg->AddEntry(hist_Mbb,leg_Mbb,"");
	leg->AddEntry(hist_Mbb_reg,leg_Mbb_reg0,"L");
	leg->AddEntry(hist_Mbb_reg,leg_Mbb_reg,"");
	leg->AddEntry(hist_Mbb_reg_fsr,leg_Mbb_reg_fsr0,"L");
	leg->AddEntry(hist_Mbb_reg_fsr,leg_Mbb_reg_fsr,"");

	leg->Draw("same");
	c1->Print("plots/v14Mbb_regressed"+type+"_cmssw76.png");
		


	return 0;
}

