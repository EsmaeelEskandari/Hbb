#include <iostream>
#include <fstream>
#include <algorithm>
#include <TH1F.h>
#include <TTree.h>
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

//int JECR_unc(int type_int, int type_jec){
int main(int argc, char* argv[]){

	int type_int = atoi(argv[1]);
	int type_jec = atoi(argv[2]);
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

	int NCATS;
	Float_t cats[10]; 
	Float_t Nevt_up[10];
	Float_t Nevt_nom[10];
	Float_t Nevt_down[10];
	Float_t SysUnc[10];
	TString cats_names[10];
	if (type_int==0) NCATS=3+1;
	else if (type_int==1) NCATS=4+1;
	if (type_int==1) {
		Float_t cats_new[10]={-1.,0.28  , 0.72  , 0.87 , 0.93    , 0.9825};
		memcpy(cats, cats_new, sizeof cats_new);
	}
	if (type_int==0) {
		Float_t cats_new[10]={-1.,0.36 ,  0.76  , 0.89  ,0.9725};
		memcpy(cats, cats_new, sizeof cats_new);
	}
	for (int i=0;i<NCATS;i++){
		TString tmp;
		tmp.Form("%1d",i);
		cats_names[i] = "CAT";
		cats_names[i].Append(tmp);
	} 
//		Float_t cats[10] = {-1.,0.12,0.44,0.57,0.6725};

	Float_t lumi = 2190.;
	Float_t xsec =  2.16;

	const int num_ss = 2;	

//	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg","GF"};
	TString s_names[num_ss] = {"vbf","GF"};
//	TString s_names[num_ss] = {"vbf_76"};
	TString tex_s_names[num_ss] = {"VBF, m(H) = 125 GeV", "GF, m(H) = 125 GeV"};
		


	TCanvas *c0 = new TCanvas();
	TFile *file_nom =  new TFile("/mnt/t3nfs01/data01/shome/nchernya/Hbb/tmva/main_mva/SysUnc/v21/main_mva_v21_"+s_names[0]+type+ToLower(jec)+"nom_v21.root");
	TTree *tree_bdt_JEnom = (TTree*)file_nom->Get(ToLower(jec)+"nom");
	TH1F *hist_bdt_JEnom= new TH1F("hist_bdt_"+jec+"_nom","",NCATS,cats);
	hist_bdt_JEnom->Scale(lumi/xsec);
	hist_bdt_JEnom->GetXaxis()->SetTitle("BDT output");
	TH1F *hist_mbb_JEnom= new TH1F("hist_mbb_"+jec+"_nom","",60,50,200);
	hist_mbb_JEnom->Scale(lumi/xsec);
	hist_mbb_JEnom->GetXaxis()->SetTitle("m_{bb} (GeV)");
	char cmd[50];
	char cmd2[50];
	char cmd3[50];
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_nom").Data(),("hist_bdt_"+jec+"_nom").Data());	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_"+jec+"_nom").Data(),("hist_mbb_"+jec+"_nom").Data());	
	sprintf(cmd2,"weight");	
	tree_bdt_JEnom->Draw(cmd,cmd2);
	tree_bdt_JEnom->Draw(cmd3,cmd2);
	for (int i=0;i<NCATS;i++)
		Nevt_nom[i] = hist_bdt_JEnom->GetBinContent(i+1);

	TCanvas *c3 = new TCanvas();
	TFile *file_down =  new TFile("/mnt/t3nfs01/data01/shome/nchernya/Hbb/tmva/main_mva/SysUnc/v21/main_mva_v21_"+s_names[0]+type+ToLower(jec)+"down_v21.root");
	TTree *tree_bdt_JEdown = (TTree*)file_down->Get(ToLower(jec)+"down");
	TH1F *hist_bdt_JEdown= new TH1F("hist_bdt_"+jec+"_down","",NCATS,cats);
	hist_bdt_JEdown->Scale(lumi/xsec);
	hist_bdt_JEdown->GetXaxis()->SetTitle("BDT output"); 
	TH1F *hist_mbb_JEdown= new TH1F("hist_mbb_"+jec+"_down","",60,50,200);
	hist_mbb_JEdown->Scale(lumi/xsec);
	hist_mbb_JEdown->GetXaxis()->SetTitle("m_{bb} (GeV)");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_down").Data(),("hist_bdt_"+jec+"_down").Data());	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_"+jec+"_down").Data(),("hist_mbb_"+jec+"_down").Data());	
	tree_bdt_JEdown->Draw(cmd,cmd2);
	tree_bdt_JEdown->Draw(cmd3,cmd2);
	for (int i=0;i<NCATS;i++)
		Nevt_down[i] = hist_bdt_JEdown->GetBinContent(i+1);


	TCanvas *c2 = new TCanvas();
	TFile *file_up =  new TFile("/shome/nchernya/Hbb/slim_trees/SysUnc/JECR/new/main_mva_v14_"+s_names[0]+"_jec_v14_cmssw76"+type+ToLower(jec)+"up.root");
	TTree *tree_bdt_JEup= (TTree*)file_up->Get(ToLower(jec)+"up");
	TH1F * hist_bdt_JEup= new TH1F("hist_bdt_"+jec+"_up","",NCATS,cats);
	hist_bdt_JEup->Scale(lumi/xsec);
	hist_bdt_JEup->GetXaxis()->SetTitle("BDT output");
	TH1F *hist_mbb_JEup= new TH1F("hist_mbb_"+jec+"_up","",60,50,200);
	hist_mbb_JEup->Scale(lumi/xsec);
	hist_mbb_JEup->GetXaxis()->SetTitle("m_{bb} (GeV)");
	sprintf(cmd,"\%s>>\%s",("BDT_VBF_"+jec+"_up").Data(),("hist_bdt_"+jec+"_up").Data());	
	sprintf(cmd3,"\%s>>\%s",("Mbb_VBF_"+jec+"_up").Data(),("hist_mbb_"+jec+"_up").Data());	
	tree_bdt_JEup->Draw(cmd,cmd2);
	tree_bdt_JEup->Draw(cmd3,cmd2);
	for (int i=0;i<NCATS;i++)
		Nevt_up[i] = hist_bdt_JEup->GetBinContent(i+1);

	
	for (int i=0;i<NCATS;i++)
		SysUnc[i] = max( TMath::Abs((Nevt_up[i] - Nevt_nom[i])), TMath::Abs((Nevt_nom[i] - Nevt_down[i])) ) / Nevt_nom[i];


	ofstream out;
	out.open("output_txt/sysUnc_"+s_names[0]+type+ToLower(jec)+".txt");
	out <<"Systematic uncertainties for "<<jec<<"  "<<s_names[0]<<"  " <<type<<endl;
	for (int i=0;i<NCATS;i++)
		out<<cats_names[i]<<"\t";
	out<<endl;	
	for (int i=0;i<NCATS;i++)
		out<<SysUnc[i]<<"\t";	
	out.close();




	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.15);
	c1->SetRightMargin(.25);
	c1->cd();
	TH1F *frame2 = new TH1F("frame2","",50,-1.,1.);
	frame2->SetMinimum(0.);
   frame2->SetMaximum(hist_bdt_JEnom->GetMaximum()*1.6);
//	if (type_int==1)  frame2->SetMaximum(0.005);
   frame2->SetStats(0);
	frame2->SetYTitle("Events");
	frame2->SetXTitle(hist_bdt_JEnom->GetXaxis()->GetTitle());
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
	c1->Print("plots/v21_vbf_"+type+"_"+jec+".png");
	

	
	TCanvas *c11 = new TCanvas();
	c11->SetBottomMargin(.15);
	c11->SetRightMargin(.25);
	c11->cd();
	TH1F *frame3 = new TH1F("frame3","",15,50.,200.);
	frame3->SetMinimum(0.);
   frame3->SetMaximum(hist_mbb_JEnom->GetMaximum()*1.6);
   frame3->SetStats(0);
	frame3->SetYTitle("Events");
	frame3->SetXTitle(hist_mbb_JEnom->GetXaxis()->GetTitle());
	frame3->GetYaxis()->SetNdivisions(505);
	frame3->GetXaxis()->SetLabelSize(0.0);
  	frame3->GetXaxis()->SetTitleSize(0.05);
  	frame3->GetXaxis()->SetLabelSize(0.04);
	frame3->Draw();
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
//	tex_file->Draw();
	hist_mbb_JEnom->SetLineColor(4);
	hist_mbb_JEnom->SetLineWidth(2);
	hist_mbb_JEup->SetLineColor(2);
	hist_mbb_JEup->SetLineWidth(2);
	hist_mbb_JEdown->SetLineColor(8);
	hist_mbb_JEdown->SetLineWidth(2);
	hist_mbb_JEnom->Draw("same");
	hist_mbb_JEup->Draw("same");
	hist_mbb_JEdown->Draw("same");
	leg->Draw("same");
	c11->Print("plots/v21_vbf_"+type+"_"+jec+"_mbb.png");




	return 0;
}

