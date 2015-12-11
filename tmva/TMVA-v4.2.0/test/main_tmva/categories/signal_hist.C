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

int signal_hist(int type_int){
	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
	Double_t precision=0.02;
	TString binning;
	binning.Form("%f",precision);
	double NCAT=6;
	double max1=0; double max2=0; double max3=0; double max4=0; double max5=0; double max6=0;
	double s1=0; double s2=0;double s3=0;  double s4=0; double s5=0; double s6=0;
	double b1=0; double b2=0;double b3=0;  double b4=0; double b5=0; double b6=0;
	double max = -10;
	double border1=0;
	double border2=0;
	double border3=0;
	double border4=0;
	double border5=0;
	double start1=-1+precision;
	double start2=-1+2*precision;
	double start3=-1+3*precision;
	double start4=-1+4*precision;
	double start5=-1+5*precision;

	
	const int num_bgs=8;  //8; //BGs
	const int num_ss=3;   //3;		//Ss

//	int type_int; //double  =0; single=1
	
	TString type;
	if (type_int==0) type = "_double";
	if (type_int==1) type = "_single";
	TString text_type;
	if (type_int==0) text_type = "Spring 15, DoubleBtag";
	if (type_int==1) text_type = "Spring 15, SingleBtag";
	

	TString bg_names[num_bgs] = {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf"};
	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg", "VBFHToBB_M-130_13TeV_powheg", "VBFHToBB_M125_13TeV_amcatnlo"};
	TString tex_s_names[num_ss] = {"VBF powheg, m(H) = 125 GeV","VBF powheg, m(H) = 130 GeV", "VBF amc@NLO, m(H) = 125 GeV"};

	TFile *file_s1 =  new TFile("output_hist/BDT_hist_"+s_names[0]+type+".root");
	TH1F *hist_S1 = (TH1F*)file_s1->Get("BDT_output");
	TFile *file_s2 =  new TFile("output_hist/BDT_hist_"+s_names[1]+type+".root");
	TH1F *hist_S2 = (TH1F*)file_s2->Get("BDT_output");
	TFile *file_s3 =  new TFile("output_hist/BDT_hist_"+s_names[2]+type+".root");
	TH1F *hist_S3 = (TH1F*)file_s3->Get("BDT_output");
	hist_S2->Rebin(10);
	hist_S1->Rebin(10);
	hist_S3->Rebin(10);


	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.12);
//	c1->SetLogy();
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",100,-1.,1.);
	frame2->SetMinimum(1e-1);
   frame2->SetMaximum(100);
   frame2->GetYaxis()->SetTitleOffset(1.4);
   frame2->GetXaxis()->SetTitleOffset(1.);
   frame2->SetStats(0);
   frame2->SetTitleFont(42,"x");
	frame2->SetTitleFont(42,"y");
   frame2->SetTitleSize(0.05, "XYZ");
	frame2->SetYTitle("Events / 10 fb^{-1}");
	frame2->SetXTitle("BDT output");	
	frame2->GetXaxis()->SetLabelSize(0.05);
	frame2->Draw();
	TLatex* tex = new TLatex(0.95,0.95,"13 TeV, PU = 20, bx = 25 ns, 10 fb^{-1}");
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
	TLatex* tex_file = new TLatex(0.42,0.95,text_type);
   tex_file->SetNDC();
	tex_file->SetTextAlign(35);
   tex_file->SetTextFont(42);
   tex_file->SetTextSize(0.04);
   tex_file->SetLineWidth(2);	
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
	tex_file->Draw();
	hist_S1->SetLineColor(2);
	hist_S1->SetLineWidth(2);
	hist_S2->SetLineColor(4);
	hist_S2->SetLineWidth(2);
	hist_S3->SetLineColor(1);
	hist_S3->SetLineWidth(2);
	hist_S3->Draw("same");
	hist_S2->Draw("same");
	hist_S1->Draw("same");
	TLegend *leg = new TLegend(0.42,0.7,0.92,0.93);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.04);
	leg->AddEntry(hist_S1,tex_s_names[0],"L");
	leg->AddEntry(hist_S2,tex_s_names[1],"L");
	leg->AddEntry(hist_S3,tex_s_names[2],"L");
	leg->Draw("same");
	c1->Print("plots/BDT_output_only_signals"+type+"_lin.png");
		


	return 0;
}

