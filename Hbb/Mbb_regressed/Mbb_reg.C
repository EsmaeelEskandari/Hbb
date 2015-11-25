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
	if (type_int==0) type = "";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "v14, DoubleBtag";
	if (type_int==1) text_type = "v14, SingleBtag";

	const int num_ss = 1;	

	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg"};
	TString tex_s_names[num_ss] = {"VBF powheg, m(H) = 125 GeV"};

	TFile *file_s1 =  new TFile("../output_hist/v14/skimmed_tree_analysis"+type+s_names[0]+"_v14_exclusive_blike_pu.root");
	TH1F *hist_Mbb = (TH1F*)file_s1->Get("hMbb");
	TH1F *hist_Mbb_reg = (TH1F*)file_s1->Get("hMbb_regVBF");
	TH1F *hist_Mbb_reg_fsr = (TH1F*)file_s1->Get("hMbb_regVBF_fsr");
	TF1 *gaus = new TF1("gaus","gaus",95,140);
	gaus->SetLineColor(4);
	gaus->SetLineWidth(1);
	
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

	
/*	Float_t Mbb_mean = hist_Mbb->GetMean(1);
	Float_t Mbb_rms = hist_Mbb->GetRMS(1);
	Float_t Mbb_reg_mean = hist_Mbb_reg->GetMean(1);
	Float_t Mbb_reg_rms = hist_Mbb_reg->GetRMS(1);
	Float_t Mbb_reg_fsr_mean = hist_Mbb_reg_fsr->GetMean(1);
	Float_t Mbb_reg_fsr_rms = hist_Mbb_reg_fsr->GetRMS(1);
*/

	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.12);
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,0.,230.);
	frame2->SetMinimum(0.);
   frame2->SetMaximum(hist_Mbb->GetMaximum()*1.6);
//	if (type_int==1)  frame2->SetMaximum(0.005);
   frame2->GetYaxis()->SetTitleOffset(1.5);
   frame2->GetXaxis()->SetTitleOffset(1.);
   frame2->SetStats(0);
   frame2->SetTitleFont(42,"x");
	frame2->SetTitleFont(42,"y");
   frame2->SetTitleSize(0.05, "XYZ");
	frame2->SetYTitle("Events / 5 GeV");
	frame2->SetXTitle("M_{bb} (GeV)");	
	frame2->GetXaxis()->SetLabelSize(0.05);
	frame2->Draw();
	TLatex* tex = new TLatex(0.95,0.95,"13 TeV, bx = 25 ns, 1 pb^{-1}");
   tex->SetNDC();
	tex->SetTextAlign(35);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   TLatex *tex1 = new TLatex(0.2,0.83,"CMS");
   tex1->SetNDC();
   tex1->SetTextAlign(20);
   tex1->SetTextFont(61);
   tex1->SetTextSize(0.06);
   tex1->SetLineWidth(2);
   TLatex* tex2 = new TLatex(0.27,0.77,"Work in progress");
   tex2->SetNDC();
   tex2->SetTextAlign(20);
   tex2->SetTextFont(52);
   tex2->SetTextSize(0.04);
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
	tex_file->Draw();
	hist_Mbb->SetLineColor(4);
	hist_Mbb->SetLineWidth(2);
	hist_Mbb_reg->SetLineColor(2);
	hist_Mbb_reg->SetLineWidth(2);
	hist_Mbb_reg_fsr->SetLineColor(8);
	hist_Mbb_reg_fsr->SetLineWidth(2);
	hist_Mbb_reg->Draw("same");
	hist_Mbb_reg_fsr->Draw("same");
	hist_Mbb->Draw("same");

	TString leg_Mbb = "Nominal";
	TString leg_Mbb_reg = "Regressed";
	TString leg_Mbb_reg_fsr = "Regressed+FSR";
//	TString leg_mean = ", Mean=";
//	TString leg_rms = ", RMS=";
	TString leg_mean = ", #mu=";
	TString leg_rms = ", #sigma=";
	TString leg_ratio = ",#sigma/#mu=";
	TString temp_str;
	temp_str.Form("%2.2f",Mbb_mean);
	leg_Mbb.Append(leg_mean);
	leg_Mbb.Append(temp_str);
	leg_Mbb.Append(leg_rms);
	temp_str.Form("%2.2f",Mbb_rms);
	leg_Mbb.Append(temp_str);
	leg_Mbb.Append(leg_ratio);
	temp_str.Form("%2.3f",Mbb_rms/Mbb_mean);
	leg_Mbb.Append(temp_str);
///////
	temp_str.Form("%2.2f",Mbb_reg_mean);
	leg_Mbb_reg.Append(leg_mean);
	leg_Mbb_reg.Append(temp_str);
	leg_Mbb_reg.Append(leg_rms);
	temp_str.Form("%2.2f",Mbb_reg_rms);
	leg_Mbb_reg.Append(temp_str);
	leg_Mbb_reg.Append(leg_ratio);
	temp_str.Form("%2.3f",Mbb_reg_rms/Mbb_reg_mean);
	leg_Mbb_reg.Append(temp_str);
////////
	temp_str.Form("%2.2f",Mbb_reg_fsr_mean);
	leg_Mbb_reg_fsr.Append(leg_mean);
	leg_Mbb_reg_fsr.Append(temp_str);
	leg_Mbb_reg_fsr.Append(leg_rms);
	temp_str.Form("%2.2f",Mbb_reg_fsr_rms);
	leg_Mbb_reg_fsr.Append(temp_str);
	leg_Mbb_reg_fsr.Append(leg_ratio);
	temp_str.Form("%2.3f",Mbb_reg_fsr_rms/Mbb_reg_fsr_mean);
	leg_Mbb_reg_fsr.Append(temp_str);

	TLegend *leg = new TLegend(0.38,0.7,0.85,0.93);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.03);
	leg->AddEntry(hist_Mbb,leg_Mbb,"L");
	leg->AddEntry(hist_Mbb_reg,leg_Mbb_reg,"L");
	leg->AddEntry(hist_Mbb_reg_fsr,leg_Mbb_reg_fsr,"L");
	leg->Draw("same");
	c1->Print("plots/v14Mbb_regressed"+type+"_fit.png");
		


	return 0;
}

