#include <iostream>
#include <fstream>
#include <TH1F.h>
#include <TH2F.h>
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
#include "TLine.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"


Bool_t reject;
Double_t erf( Double_t *x, Double_t *par){
if (reject && x[0] > 150 && x[0] < 180) {
      TF1::RejectPoint();
      return 0;
   }
  return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2]));
}

Double_t erf2( Double_t *x, Double_t *par){
  return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2])) + (1.-par[0]);
}

Double_t erf2_2d(Double_t *x, Double_t *par){
//	Double_t *p1=&par[0];
//	Double_t *p2=&par[3];
//	return erf2(x,p1)*erf2(x,p2);
//	return erf(x[0],p1)*erf(x[1],p2);
	if (x[1]>x[0]) return 0;
	else return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2]))* (par[3]/2.*(1.+TMath::Erf((x[1]-par[4])/par[5])) );
}





using namespace std;

int main(int argc, char* argv[]){
//int plotTurnOn(int set_type=0,int trigger=0, int n_1=1){
	int set_type=atoi(argv[1]);
	int trigger=atoi(argv[2]);
	int n_1=atoi(argv[3]); // 0 -false, 1 - true -> n_1 plots
	int histFit0 = atoi(argv[4]); //btag log  9 and 10
	int signal_sample_num = 0;

	TString n_1_str_dir[2] = {"",""};
//	TString n_1_str_end[2] = {"","_n_1_tighter"};
	//TString n_1_str_end[2] = {"","_n_1"};
//	TString n_1_str_end[2] = {"","_btagW_123jet"};
	TString n_1_str_end[2] = {"","_btagW_1jet"};
	
	TString n_1_name[2] = {"N cut trg effi.","(N-1) cut trg effi."};

	Double_t presel_cuts[20];
	if (set_type==0) { presel_cuts = {92.,76.,64.,30.,200.,0.,0.,1.2,2.4};}
	 if (set_type==1){ presel_cuts = {92.,76.,64.,30.,460.,0.,0.,4.1,1.6};}
//	if (set_type==0) { Double_t presel_cuts[20]= {92.,76.,64.,30.,200.,0.,0.,1.2,2.4};}
//	 if (set_type==1){ Double_t presel_cuts[20]= {92.,76.,64.,30.,460.,0.,0.,4.1,1.6};}


	gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");


	const int num_bgs=8;  //8; //BGs
	const int num_ss=3;   //3;		//Ss

//	int type_int; //double  =0; single=1

	TString type[2] = {"_double","_single"};
//	TString text_type[2] = {"v13, DoubleBtag","v13, SingleBtag"};
	TString text_type[2] = {"HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200","HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460"};
	TString dir_type[2] = {"DoubleBtag/","SingleBtag/"};
	TString dir_trigger[3];
	TString denominator_trigger_names[4] = { "HLT_BIT_HLT_QuadPFJet_VBF_v","HLT_BIT_HLT_DiPFJetAve60_v","HLT_BIT_HLT_DiPFJetAve40_v","HLT_BIT_HLT_DiPFJetAve40or60_v"};
	TString denominator_trigger_names_data[4] = { "HLT_BIT_HLT_QuadPFJet_VBF_v","HLT_BIT_HLT_DiPFJetAve60_v","HLT_BIT_HLT_DiPFJetAve40_v","HLT_BIT_HLT_DiPFJetAve40or60_v"};
	TString denominator_trigger_names_str[4] = { "HLT_BIT_HLT_QuadPFJet_VBF_v","HLT_BIT_HLT_DiPFJetAve60_v","HLT_BIT_HLT_DiPFJetAve40_v","HLT_BIT_HLT_DiPFJetAve40or60_v"};
	for (int i=0;i<3;i++){
		dir_trigger[i]=denominator_trigger_names[i];
		dir_trigger[i].Append("/");
	}

	TString bg_names[num_bgs] = {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf"};
	TString s_names[num_ss] = {"VBFHToBB_M-125_13TeV_powheg"};
	TString tex_s_names[num_ss] = {"VBF powheg, m(H) = 125 GeV #times 0.25"};
	TString data_names[3] = {"JetHT_D"};
	
   const int numArray= 11;
	TString hist_names[numArray] = {"hJet1_pt_trigger","hJet2_pt_trigger","hJet3_pt_trigger","hJet4_pt_trigger", "hMqq_trigger", "hbtag_trigger", "hbtag2_trigger","hDeltaEta_trigger","hDeltaPhi_trigger", "hbtag_log_trigger", "hbtag2_log_trigger"};
	TString hist_names_notrigger[numArray] = {"hJet1_pt","hJet2_pt","hJet3_pt","hJet4_pt", "hMqq", "hbtag", "hbtag2","hDeltaEta","hDeltaPhi","hbtag_log", "hbtag2_log"};

for (int current_hist=0;current_hist<numArray;current_hist++){

//	TFile *file_s =  new TFile("output_hist/"+n_1_str_dir[n_1]+s_names[signal_sample_num]+type[set_type]+denominator_trigger_names[trigger]+n_1_str_end[n_1]+".root");
//	TH1F *hist_S = (TH1F*)file_s->Get(hist_names[current_hist]);
//	TH1F *hist_S_notr = (TH1F*)file_s->Get(hist_names_notrigger[current_hist]);

//	TH2F *hist_S_csv = (TH2F*)file_s->Get("hCSV_trigger2");
//	TH2F *hist_S_csv_notr = (TH2F*)file_s->Get("hCSV2");

	TFile *file_data =  new TFile("output_hist/"+n_1_str_dir[n_1]+data_names[0]+type[set_type]+denominator_trigger_names_data[trigger]+n_1_str_end[n_1]+".root");
	TH1F *hist_Data = (TH1F*)file_data->Get(hist_names[current_hist]);
	TH1F *hist_Data_notr = (TH1F*)file_data->Get(hist_names_notrigger[current_hist]);
	TH2F *hist_Data_csv = (TH2F*)file_data->Get("hCSV_trigger");
	TH2F *hist_Data_csv_notr = (TH2F*)file_data->Get("hCSV");
	

	TFile *file_b[num_bgs];
	TH1F *hist_Bs[num_bgs];
	TH1F *hist_Bs_notr[num_bgs];
	TH2F *hist_Bs_csv[num_bgs];
	TH2F *hist_Bs_csv_notr[num_bgs];
		
	Float_t lumi = 2320;

	file_b[0] = new TFile("output_hist/"+n_1_str_dir[n_1]+bg_names[0]+type[set_type]+denominator_trigger_names[trigger]+n_1_str_end[n_1]+".root");
	hist_Bs[0] = (TH1F*)file_b[0]->Get(hist_names[current_hist]);
	hist_Bs_notr[0] = (TH1F*)file_b[0]->Get(hist_names_notrigger[current_hist]);
	hist_Bs_csv[0] = (TH2F*)file_b[0]->Get("hCSV_trigger");
	hist_Bs_csv_notr[0] = (TH2F*)file_b[0]->Get("hCSV");
	for (int i=1;i<num_bgs;i++){
		file_b[i] = new TFile("output_hist/"+n_1_str_dir[n_1]+bg_names[i]+type[set_type]+denominator_trigger_names[trigger]+n_1_str_end[n_1]+".root");
		hist_Bs[i] = (TH1F*)file_b[i]->Get(hist_names[current_hist]);
		hist_Bs_notr[i] = (TH1F*)file_b[i]->Get(hist_names_notrigger[current_hist]);
		hist_Bs[0]->Add(hist_Bs[i]);
		hist_Bs_notr[0]->Add(hist_Bs_notr[i]);
		hist_Bs_csv[i] = (TH2F*)file_b[i]->Get("hCSV_trigger");
		hist_Bs_csv_notr[i] = (TH2F*)file_b[i]->Get("hCSV");
		hist_Bs_csv[0]->Add(hist_Bs_csv[i]);
		hist_Bs_csv_notr[0]->Add(hist_Bs_csv_notr[i]);
	}

	hist_Data->Divide(hist_Data_notr);
//	hist_S->Divide(hist_S_notr);
	hist_Bs[0]->Divide(hist_Bs_notr[0]);
	TH1F *hist_B = (TH1F*)hist_Bs[0]->Clone();
	hist_Data_csv->Divide(hist_Data_csv_notr);
	hist_Bs_csv[0]->Divide(hist_Bs_csv_notr[0]);
	TH2F *hist_B_csv = (TH2F*)hist_Bs_csv[0]->Clone();


	
/*
	if ((current_hist==0)||(current_hist==1)) {
		TF1 *func = new TF1("erffunc",erf,0.,1000.,3);
	//	TF1 *func = new TF1("erffunc",erf3,0.,1000.,3);
	//	func->SetParameters(3.,0.1,92.);
		if (set_type==1) func->SetParameters(0.1,2,1);
		if (set_type==0) func->SetParameters(0.1,92,100);
		hist_Data->Fit(func,"R");
		cout<<"Data : Chi^2 = " <<func->GetChisquare() <<endl;
		hist_B->Fit(func,"R");
		cout<<"Data : Chi^2 = " << func->GetChisquare() <<endl;

	}*/


		TH1F* hist_Ratio = (TH1F*)hist_Data->Clone("new");
		hist_Ratio->Divide(hist_B);
		TH1F* hist_Ratio_up = (TH1F*)hist_Ratio->Clone("up");
		TH1F* hist_Ratio_down= (TH1F*)hist_Ratio->Clone("down");
	
  		for(int i=0;i<hist_Ratio->GetNbinsX();i++) {
    		float ey = 0.0;
    		float bin = 0.0;
   		bin = hist_Ratio->GetBinContent(i+1); 
      	ey = hist_Ratio->GetBinError(i+1);
    		hist_Ratio_up->SetBinContent(i+1,bin+ey);
    		hist_Ratio_down->SetBinContent(i+1,bin-ey);
  		}	
		TH2F* hist_Ratio_csv = (TH2F*)hist_Data_csv->Clone("new");
		hist_Ratio_csv->Divide(hist_B_csv);

		int histFit=histFit0; //10

		TF1 func_r("erffunc",erf,0.,4000.,3);
		float par1, par2, par0;
		float par1_up, par1_down, par2_up, par2_down, par0_up, par0_down;
		if (histFit==0)	{par0 = .7; par1 = 85.; par2 = 25.; } // jet 1
		if (histFit==1)	{par0 = .7; par1 = 85.; par2 = 15.; } // jet 2
		if (histFit==2)	{par0 = .7; par1 = 60.; par2 = 15.; } // jet 3
		if (histFit==3)	{par0 = .7; par1 = 30.; par2 = 15.; } // jet 4
		par2_down = 7.; par2_up = 39.;
		par0_down = 0.8; par0_up =  1.3;
		if ((histFit==9)||(histFit==10)) {
			par0 = .7; par1 = 2.;  par2 = 5;
			par2_down = 1.; par2_up = 20.;
			par0_down = 0.6; par0_up =  1.3;
		}
		float trigCor1_nom_single[10] = {8.24458e+01,7.95354e+01 ,5.29763e+01};
		float trigCor2_nom_single[10] = { 2.90000e+01,1.45915e+01 , 1.62510e+01};

		float trigCor1_nom_double[10] = {7.60656e+01, 7.14527e+01,  3.78975e+01  };
		float trigCor2_nom_double[10] = {2.90000e+01,1.68348e+01, 3.71160e+01 };
		func_r.SetParameters(par0,par1,par2);
		if (set_type==0)	func_r.FixParameter(1,trigCor1_nom_double[current_hist] );
		if (set_type==0)	func_r.FixParameter(2,trigCor2_nom_double[current_hist] );
		if (set_type==1)	func_r.FixParameter(1,trigCor1_nom_single[current_hist] );
		if (set_type==1)	func_r.FixParameter(2,trigCor2_nom_single[current_hist] );
	//	func_r.SetParameters(par0,par1,par2);
	//	func_r.SetParLimits(2,par2_down,par2_up);
	//	func_r.SetParLimits(0,par0_down,par0_up);

		if ((current_hist==histFit)) {
			hist_Ratio->Fit(&func_r);
		}

		TF1 func_r_up("erffuncup",erf,0.,4000.,3);
		func_r_up.SetLineWidth(1);
		func_r_up.SetLineColor(kRed-7);
		func_r_up.SetFillColor(kRed-7);
		func_r_up.SetFillStyle(3004);
		func_r_up.SetParameters(func_r.GetParameter(0)*1.1,func_r.GetParameter(1)*0.8,func_r.GetParameter(2)*1.1);
	//	func_r_up.SetParLimits(0,func_r.GetParameter(0)*1.05,func_r.GetParameter(0)*1.2); 
		func_r_up.FixParameter(0,func_r.GetParameter(0)); 
		func_r_up.SetParLimits(2,func_r.GetParameter(2)*1.05,func_r.GetParameter(2)*1.2); 
		if (current_hist<4)func_r_up.SetParLimits(1,func_r.GetParameter(1)*0.75,func_r.GetParameter(1)*0.9); 
		TF1 func_r_down("erffuncdown",erf,0.,4000.,3);
		func_r_down.SetLineWidth(1);
		func_r_down.SetLineColor(kRed-7);
		func_r_down.SetFillColor(10);
		func_r_down.SetFillStyle(1001);
		func_r_down.SetParameters(func_r.GetParameter(0)*0.9,func_r.GetParameter(1)*1.2,func_r.GetParameter(2)*.9);
	//	func_r_down.SetParLimits(0,func_r.GetParameter(0)*0.8,func_r.GetParameter(0)*0.95); 
		func_r_down.FixParameter(0,func_r.GetParameter(0)); 
		func_r_down.SetParLimits(2,func_r.GetParameter(2)*0.8,func_r.GetParameter(2)*.95); 
		if (current_hist<4)func_r_down.SetParLimits(1,func_r.GetParameter(1)*1.1,func_r.GetParameter(1)*1.25); 
		if ((current_hist==histFit)) {
			hist_Ratio_up->Fit(&func_r_up);
			hist_Ratio_down->Fit(&func_r_down);
		}
		reject = kFALSE;//change for Double Mqq for kTRUE
/*
		TF1 func_r("erffunc",erf2,0.,4000.,3); //+offset for fitting SingleBtag -log(1-csv1)
		func_r.SetParameters(0.4,2.1,0.6);// //singleB csv1, csv2
		func_r.SetParameters(0.9,96.,17);// single jet 1
		func_r.SetParameters(0.9,83.,26);// single jet 2
*/


	Float_t QCD_denominator = 0.;
	Float_t QCD_nominator = 0.;
	Float_t data_denominator = 0.;
	Float_t data_nominator = 0.;


	

	Float_t tmp=0.;
	for (int i=0;i<num_bgs;i++){
		ifstream in;
		in.open("output_txt/"+n_1_str_dir[n_1]+bg_names[i]+type[set_type]+denominator_trigger_names[trigger]+n_1_str_end[n_1]+".txt");
		in >> tmp;
		QCD_nominator+=tmp; 
		tmp=0.;
		in >> tmp;
		QCD_denominator+=tmp; 
		in.close();	
	}
	tmp=0;
	ifstream in;
	in.open("output_txt/"+n_1_str_dir[n_1]+data_names[0]+type[set_type]+denominator_trigger_names_data[trigger]+n_1_str_end[n_1]+".txt");
	in >> tmp;
	data_nominator+=tmp; 
	tmp=0.;
	in >> tmp;
	data_denominator+=tmp; 
	in.close();

	Float_t tsf = (data_nominator/data_denominator)/(QCD_nominator/QCD_denominator);
	Float_t data_eff = (data_nominator/data_denominator);
	Float_t mc_eff = (QCD_nominator/QCD_denominator);
	Float_t data_eff_unc = TMath::Sqrt((1-data_eff)*data_eff/(data_denominator-1));
	Float_t mc_eff_unc = TMath::Sqrt((1-mc_eff)*mc_eff/(QCD_denominator-1));
	Float_t tsf_eff_unc = TMath::Sqrt( pow(mc_eff_unc/mc_eff,2) + pow(data_eff_unc/data_eff,2) ) * tsf;
	ofstream out_eff;
	out_eff.open("output_txt/Efficiency"+n_1_str_dir[n_1]+data_names[0]+type[set_type]+denominator_trigger_names_data[trigger]+n_1_str_end[n_1]+".txt");
	if (current_hist==0) out_eff<<data_nominator/data_denominator<< " +- "<<data_eff_unc <<"  ;  "<<QCD_nominator/QCD_denominator<<" +- "<<mc_eff_unc<<" ;  " <<tsf<< "  +-  "<< tsf_eff_unc<<endl;
	if (current_hist==0)cout<<data_nominator/data_denominator<< " +- "<<data_eff_unc <<"  ;  "<<QCD_nominator/QCD_denominator<<" +- "<<mc_eff_unc<<" ;  " <<tsf<< "  +-  "<< tsf_eff_unc<<endl;
	out_eff.close();

	TCanvas *c1 = new TCanvas();
	c1->SetBottomMargin(.09);
//	c1->SetLogy();
	c1->cd();
 		TPad *pad1 = new TPad("pad1","pad1", 0, 0.3, 1, 1.0);
   	pad1->SetBottomMargin(0); // Upper and lower plot are joined
    	pad1->SetBottomMargin(.02);
		pad1->SetTopMargin(0.06);
		pad1->SetTicks(1,1);
    	pad1->Draw();             // Draw the upper pad: pad1
   	pad1->cd();
	Float_t x_min = hist_Data->GetBinCenter(1)*0.95;
	Float_t x_max = hist_Data->GetBinCenter(hist_B->GetSize())*1.05;
//	Float_t y_min =hist_B->GetBinContent(hist_B->GetMinimumBin())*0.9 ;
//	if (y_min==0) y_min = 1e-3;
//	Float_t y_max =hist_B->GetBinContent(hist_B->GetMaximumBin())*1.1 ;
	Float_t y_min =hist_Data->GetMinimum()*0.9 ;
	Float_t y_max =hist_Data->GetMaximum()*3. ;
	//if (y_min==0) y_min = 1e-3;
	TH1F *frame2 = new TH1F("frame2","",1.,x_min,x_max);
	frame2->SetMinimum(y_min);
   frame2->SetMaximum(y_max);
   frame2->GetYaxis()->SetTitleOffset(0.9);
   frame2->GetXaxis()->SetTitleOffset(2.);
   frame2->GetXaxis()->SetLabelOffset(2.);
   frame2->SetStats(0);
//	frame2->GetXaxis()->SetNdivisions(0);
   frame2->SetTitleFont(42,"x");
	frame2->SetTitleFont(42,"y");
   frame2->SetTitleSize(0.06, "XYZ");
	frame2->SetYTitle("Efficiency");
	frame2->GetXaxis()->SetLabelSize(0.05);
  //		frame2->GetYaxis()->SetLabelSize(.09);
  //  	frame2->GetXaxis()->SetLabelSize(.12);	
	frame2->Draw();
	TLatex* tex = new TLatex(0.95,0.95,"13 TeV, PU = 20, bx = 25 ns, 2.32 fb^{-1}");
   tex->SetNDC();
	tex->SetTextAlign(35);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   TLatex *tex1 = new TLatex(0.2,0.8,"CMS");
   tex1->SetNDC();
   tex1->SetTextAlign(20);
   tex1->SetTextFont(61);
   tex1->SetTextSize(0.06);
   tex1->SetLineWidth(2);
   TLatex* tex2 = new TLatex(0.27,0.74,"Work in progress");
   tex2->SetNDC();
   tex2->SetTextAlign(20);
   tex2->SetTextFont(52);
   tex2->SetTextSize(0.04);
  	tex2->SetLineWidth(2);
	TLatex* tex_file = new TLatex(0.42,0.95,denominator_trigger_names_str[trigger]);
   tex_file->SetNDC();
	tex_file->SetTextAlign(35);
   tex_file->SetTextFont(42);
   tex_file->SetTextSize(0.04);
   tex_file->SetLineWidth(2);
	TLatex* tex_tr = new TLatex(0.5,0.87,text_type[set_type]);
   tex_tr->SetNDC();
	tex_tr->SetTextAlign(35);
   tex_tr->SetTextFont(61);
   tex_tr->SetTextSize(0.04);
   tex_tr->SetLineWidth(2);

	TLatex *tex_n_1;
	if ((current_hist!=5)&&(current_hist!=6)) tex_n_1= new TLatex(0.33,0.87,n_1_name[n_1]);
	else  tex_n_1= new TLatex(0.35,0.87,n_1_name[0]);
   tex_n_1->SetNDC();
	tex_n_1->SetTextAlign(35);
   tex_n_1->SetTextFont(42);
   tex_n_1->SetTextSize(0.04);
   tex_n_1->SetLineWidth(2);


	TLine *line = new TLine(presel_cuts[current_hist],(double)y_min,presel_cuts[current_hist],(double)y_max);
	line->SetLineStyle(9);
	line->SetLineColor(8);
	line->SetLineWidth(3);
	if ((n_1)&&(current_hist!=5)&&(current_hist!=6)&&(current_hist!=9)&&(current_hist!=10)){
		line->Draw();
	}

	tex_tr->Draw();
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
	tex_file->Draw();
	tex_n_1->Draw();

	hist_B->SetLineColor(2);
	hist_B->SetMarkerStyle(26);
	hist_B->SetMarkerColor(2);
	hist_Data->SetLineColor(1);
	hist_Data->SetMarkerStyle(20);
	hist_Data->SetMarkerColor(1);
//	hist_S->SetLineColor(4);
//	hist_S->SetMarkerStyle(23);
//	hist_S->SetMarkerColor(4);
	hist_Ratio->SetLineColor(1);
	hist_Ratio->SetMarkerStyle(20);
	hist_Ratio->SetMarkerColor(1);
//	hist_S->Draw("same");
	hist_B->Draw("same");
	hist_Data->Draw("same");



	TLegend *leg = new TLegend(0.48,0.68,0.9,0.83);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextSize(0.035);
//	leg->AddEntry(hist_S,tex_s_names[signal_sample_num],"LP");
	leg->AddEntry(hist_B,"QCD","LP");
	leg->AddEntry(hist_Data,"JetHT Data","LP");
	leg->Draw("same");


		c1->cd();          // Go back to the main canvas before defining pad2
   	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    	pad2->SetTopMargin(.13);
    	pad2->SetBottomMargin(.4);
    	pad2->Draw();
    	pad2->cd();

		TH1F *frame3 = new TH1F("frame3","",1,x_min,x_max);
		frame3->SetMinimum(0.);
      frame3->SetMaximum(2.);
		if ((histFit==9)||(histFit==10)){
			frame3->SetMinimum(0.5);
      	frame3->SetMaximum(1.5);
		}
      frame3->GetYaxis()->SetTitleOffset(0.4);
      frame3->GetXaxis()->SetTitleOffset(1.3);
      frame3->SetStats(0);
		frame3->SetXTitle(hist_Data->GetXaxis()->GetTitle());
      frame3->SetTitleFont(42,"x");
		frame3->SetTitleFont(42,"y");
      frame3->SetTitleSize(0.13, "XYZ");
	//	frame3->GetYaxis()->SetNdivisions(505);
  		frame3->GetYaxis()->SetLabelSize(.09);
    	frame3->GetXaxis()->SetLabelSize(.12);	
		frame3->SetYTitle("Data / MC");
		frame3->Draw();	

		func_r_up.Draw("same");
		func_r_down.Draw("same");

 		Float_t aa[2] = {x_min,x_max};
   	Float_t bb[2] = {1,1};
   	TGraph *cons = new TGraph(2,aa,bb);
    	cons->SetLineStyle(2);
		cons->Draw("Lsame");

		hist_Ratio->Draw("PEsame");

	//	pad1->RedrawAxis();
		pad2->RedrawAxis();



		if (current_hist==histFit) c1->Print("plots/onebyone_newunc/"+hist_names[current_hist]+denominator_trigger_names[trigger]+type[set_type]+n_1_str_end[n_1]+".png");


	if ((current_hist==numArray-1)||(current_hist==numArray-2)||(current_hist==numArray-3)){
		TF2 *f2d =new TF2("f2d",erf2_2d,2.,10.,2.,10.,6);
		f2d->SetParameters(3.,9,2,3.,9,2);
		f2d->SetParLimits(0,2.,4.);
		f2d->SetParLimits(1,7.,10);
		f2d->SetParLimits(2,1.,3.);
		f2d->SetParLimits(3,2.,4.);
		f2d->SetParLimits(4,7.,10.);
		f2d->SetParLimits(5,1.,3.);
		//hist_Ratio_csv->Fit(f2d,"R");
		TCanvas *c2 = new TCanvas();
		c2->SetBottomMargin(.09);
		c2->SetRightMargin(.1);
//		Float_t x_min2 = 0.7;//hist_Data_csv->GetBinCenter(1)*0.95;
//		Float_t x_max2 = hist_Data_csv->GetBinCenter(hist_Data_csv->GetSize())*1.05;
	//	TH1F *frame4 = new TH1F("frame4","",1,0.,1);
		TH1F *frame4 = new TH1F("frame4","",1,0.,7.);
		hist_Data_csv->SetTitle("Data"); hist_Data_csv->SetStats(kFALSE);
		hist_B_csv->SetTitle("MC");hist_B_csv->SetStats(kFALSE);
		hist_Ratio_csv->SetTitle("Ratio");hist_Ratio_csv->SetStats(kFALSE);
	//	frame4->SetMinimum(0.0);
		frame4->SetMaximum(7.);
		frame4->Draw();
		if (current_hist==numArray-3) {
	//		hist_Data_csv->Draw("same");
			hist_Data_csv->Draw("colz1 texte same");
//			c2->Print("plots/onebyone/CSV1log_map_Data"+denominator_trigger_names[trigger]+type[set_type]+n_1_str_end[n_1]+".png");
		}
		if (current_hist==numArray-2) {
		//	hist_B_csv->Draw("same");
			hist_B_csv->Draw("colz1 texte same");
//			c2->Print("plots/onebyone/CSV1log_map_MC"+denominator_trigger_names[trigger]+type[set_type]+n_1_str_end[n_1]+".png");
		}
		if (current_hist==numArray-1) {
		//	hist_Ratio_csv->Draw("same");
			hist_Ratio_csv->Draw("colz1 texte same");
	//		hist_Ratio_csv->Draw("Surf1");
  		//	f2d->Draw("surf1 same");
//			c2->Print("plots/onebyone/CSV1log_map_Ratio"+denominator_trigger_names[trigger]+type[set_type]+n_1_str_end[n_1]+".png");		
			c2->Delete();
		}
	}
}

	

return 0;

}
