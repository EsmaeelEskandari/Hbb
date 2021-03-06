#include <stdio.h>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TTree.h"
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TCut.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TF1.h>
#include "TROOT.h"
#include "TPad.h"
#include <boost/format.hpp>
#include <array>
#include <algorithm>
#include <iterator>

#define SWAP(A, B) { Float_t t = A; A = B; B = t; }
#define SWAP2(A, B) { B.swap(A); }

void bubblesort(Float_t *a, std::string *str, int n)
{
  int i, j;
  for (i = n - 1; i >= 0; i--)
  {
    for (j = 0; j < i; j++)
    {
      if (a[j] < a[j + 1]){
        SWAP( a[j], a[j + 1] );
	//	cout<<str[j]<<"   "<<str[j+1]<<endl;
		SWAP2( str[j], str[j+1]);
	//	cout<<str[j]<<"   "<<str[j+1]<<endl;
		}
    }
  }
}


Float_t Discr(TH1F *h1, TH1F *h2){  
  Int_t h1s = h1->GetNbinsX();
  Int_t h2s = h2->GetNbinsX();
  if (h1s != h2s) {
  	printf("h1 size %d   h2 size %d \n",h1s,h2s);
  	return -1;
  }
  if (h1->Integral()!=0 ) h1->Scale(1./h1->Integral());
  if (h2->Integral()!=0 ) h2->Scale(1./h2->Integral());
  Float_t adiff = 0;
  for (Int_t i=0;i<h1s;i++){
	adiff +=  TMath::Abs(h1->GetBinContent(i) - h2->GetBinContent(i));
	//cout << "h1 bin " << i << " " << h1->GetBinContent(i) << endl;
	}
  return adiff/2;
}

using namespace std;

int main(int argc, char* argv[]){
//int analyzer_stackRatio(){
gROOT->ProcessLine(".x /afs/cern.ch/work/n/nchernya/setTDRStyle.C");
int set_type = atoi(argv[1]); //0 - double, 1 - single
int region_type=atoi(argv[2]); // 0 - analysis, 1 - control region , top
 
const int nfiles  = 18;
TString leg_names[nfiles] = {"Data",/*"powheg, m(H) = 130 GeV","amc@NLO, m(H) = 125 GeV",*/"VBF, m(H) = 125 GeV","GF, m(H) = 125 GeV","t#bar{t}H, H#rightarrow b#bar{b}","t#bar{t}H, non b#bar{b}","Z(ll) + jets","W(l#nu) + jets","Single t (tW)","Z + jets","t#bar{t}","QCD, H_{T}>100 GeV","QCD, H_{T}>200 GeV","QCD, H_{T}>300 GeV","QCD, H_{T}>500 GeV","QCD, H_{T}>700 GeV","QCD, H_{T}>1000 GeV","QCD, H_{T}>1500 GeV","QCD, H_{T}>2000 GeV"};
if (set_type==0)leg_names[0] = "Data (DoubleB)";
if (set_type==1)leg_names[0] = "Data (SingleB)";


TString file_names[nfiles] = { "BTagCSV_golden",/*"VBFHToBB_M-130_13TeV_powheg",*/"VBFHToBB_M-125_13TeV_powheg","GF","ttHtobb","ttHtoNbb","DYtoLL","WJetsToLNu","ST_tW","DYtoQQ","TTbar","QCD_HT2000toInf","QCD_HT1500to2000","QCD_HT1000to1500","QCD_HT100to200","QCD_HT700to1000","QCD_HT200to300","QCD_HT500to700","QCD_HT300to500"}; 
int bg_begin;
int qcd_begin=10;
if (region_type==0) bg_begin=7;
else bg_begin=5;

int FILLCOLOR[nfiles] = {1,kRed,kBlue+0,kViolet+2,kGreen+2,kYellow-4,kYellow-3,kOrange-9,kYellow-10,kOrange-2,kRed+2,kRed+1,kRed+0, kRed-7,kRed-9,kRed-6,kRed-8,kRed-10};
int LINECOLOR[nfiles] = {1,kRed,kBlue+0,kViolet+2,kGreen+2,1,1,1,1,1,1,1,1,1,1,1,1,1};
int LINESTYLE[nfiles] = {1,1,2,3,8,1,1,1,1,1,1,1,1,1,1,1,1,1};
int LINEWIDTH[nfiles] = {1,3,3,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1};
int FILLSTYLE[nfiles] = {1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001};
	
int order[nfiles] = {0,1,2,3,4, 5, 6, 7, 8, 9,17,16,15,10,14,11,13,12};// number in file_names array as they should appear in the legend
int order_legend[nfiles]; 
for (int i=0;i<nfiles;i++){
	order_legend[order[i]]=i;
}
	

TString btag[2] = {"v14, DoubleBtag","v14, SingleBtagBlike"}; 
TString region[3]={"_analysis","_controlTop","_controlDY"}; // 0 - analysis, 1 - control region , top; 2 - control region DY

TString set[2] = {"_double","_single"};
for (int i=0;i<nfiles;i++){
	file_names[i].Prepend(set[set_type]);
	file_names[i].Prepend(region[region_type]);
	file_names[i].Prepend("skimmed_tree");
	file_names[i].Prepend("../../output_hist/v14/golden/");
//	file_names[i].Append("_v14_final_74cmssw");
//	file_names[i].Append("_v14_bdt");  //without trigger correction
	file_names[i].Append("_v14_topCR");  //without trigger correction
	file_names[i].Append(".root");
}
TString trigger[2] = {"RatioDoubleBtag_blike_", "RatioSingleBtag_blike_"};
//TString trigger[2] = {"trigWeightRatioDoubleBtag_", "trigWeightSilvioRatioSingleBtag_"};
//TString dir_name= "plots_powheg_130/";
TString dir_name= "plots_powheg_125_exclusive";
dir_name.Append(region[region_type]+"/");
//TString dir_name = "plots_amc/";
Float_t lumi = 2190;

	


TLegend *leg = new TLegend(0.77,0.45,0.92,0.9); //without writing about SF
leg->SetFillColor(0);
leg->SetBorderSize(0);
leg->SetTextFont(42);
leg->SetTextSize(0.025);
TLegend *leg2 = new TLegend(0.77,0.35,0.92,0.4); //with  writing about SF
leg2->SetFillColor(0);
leg2->SetBorderSize(0);
leg2->SetTextFont(42);
leg2->SetTextSize(0.025);

const int nhistos = 74; //40//52
TString hist_names[nhistos]={"hJet1_pt","hJet2_pt","hJet3_pt","hJet4_pt","hJet1_eta","hJet2_eta","hJet3_eta","hJet4_eta","hJet1_phi","hJet2_phi","hJet3_phi","hJet4_phi","hMqq", "hEtaQQ", "hPhiBB","hMbb","hbtag","hbtag2","hcosOqqbb","hEtaQB1", "hEtaQB2", "hPhiQB1", "hPhiQB2","hx1","hx2","hVB1_mass","hVB2_mass","hEtot","hPxtot","hPytot","hPztot","hJet5_pt","hPtqqbb","hEtaqqbb","hPhiqqbb","hJet1_pt_bin","hJet2_pt_bin","hJet3_pt_bin","hJet4_pt_bin", "hMqq_bin","hEtaSoftJets", "hPtSoftJets","hMassSoftJets","hHTsoft","hSoft_n2","hSoft_n5","hSoft_n10","hqgl","hqgl2", "hPtSoftJets2","hPtSoftJets3","hPVs", "hJet1q_pt", "hJet1q_eta", "hJet1q_ptd", "hJet1q_axis2", "hJet1q_mult", "hJet2q_pt", "hJet2q_eta", "hJet2q_ptd", "hJet2q_axis2", "hJet2q_mult","hMbb_regVBF","hMbb_regVBF_fsr","hblike1","hblike2", "hmet", "hqgl1_VBF", "hqgl2_VBF", "hbdt", "hPhiQQ", "hvLeptons_dz", "hvLeptons_edz", "hvLeptons_sig"    /*, "hMbb_regVBF_fsr_high", "hMbb_regVBF_fsr_high_cat"*/};
int UNITS[nhistos]={1,1,1,1 ,0,0,0,0 ,0,0,0,0, 1 ,0,0 ,1, 0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1 ,0,0, 1,1,1,1,1, 0, 1,1,1 ,0,0,0,0,0, 1,1, 0, 1,0,0,0,0, 1,0,0,0,0, 1,1 ,0,0, 1, 0,0,0,0/*,1,1*/};
std::array<int,100> LOGY_array = {71,72,51,52,53,54,55,56,57,58,59,60,61};

TString hist_names_sum[nhistos]={};
TString sum_histos_names[nhistos]={};
std::string hist_names_sort[nhistos];
for (int i=0;i<nhistos;i++){
	hist_names_sort[i] = hist_names[i];
	hist_names_sum[i] = hist_names[i];
	hist_names_sum[i].Append("_sum");
	sum_histos_names[i] = hist_names[i];
	sum_histos_names[i].Append("_sum0");
}
Float_t discriminators[nhistos];
TString stacks_names[nhistos];
for (int i=0;i<nhistos;i++){
	stacks_names[i] = hist_names[i];
	stacks_names[i].Prepend("s");
}
TString output_names[nhistos];
for (int i=0;i<nhistos;i++){
	output_names[i] = hist_names[i];
	output_names[i].Prepend(dir_name);
	output_names[i].Prepend("wo_trigger_corr/"+trigger[set_type]);
	output_names[i].Append(region[region_type]);
	output_names[i].Append(set[set_type]+"_v14_final_74cmssw_wotriggercorr.png");
}

TH1F *data_histos[nhistos];
TH1F *data_histos2[nhistos];
TH1F *signal_histos[nhistos];
TH1F *signal_histos2[nhistos];
TH1F *tthbb_histos[nhistos];
TH1F *tthnbb_histos[nhistos];
TH1F *gf_histos[nhistos];
TH1F *sum_histos[nhistos];
TH1F *histos_forUnc[nhistos];
TH1F *histos_for_legened[nhistos];
TH1F *discr_histos[nhistos];//Mqq,delta eta, delta phi, qgl, btag //12,13,14,21,22
TH1F *hBkgVis[nhistos];
TH1F *hBkgUncUp[nhistos];
TH1F *hBkgUncLo[nhistos];


int files=0; 
THStack *stacks[nhistos];
for (int i=0;i<nhistos;++i){
	stacks[i] = new THStack(stacks_names[i],"");
}
Double_t totalBG=0.;
Double_t totalQCD=0.;
Double_t totalMC=0.;
Double_t totalData=0.;
Double_t totalDataQCD=0.;
ofstream out_efficiency;
ofstream out_discrimination;
out_efficiency.open(trigger[set_type]+dir_name+"efficiency.txt"); 

//Float_t MC_data[2] = {1.,1.};////////////for efficiency 
Float_t MC_data[2] = {1./0.7581,1./0.906};////////////for blike single btag logic and with PU for golden
//Float_t MC_data[2] = {1./1.5,1./1.56};////////////trigger correction using 4 jets, mqq(double) and csv1,csv2(double)
//Float_t MC_data[2] = {1./1.07,1./1.52};////////////trigger correction using 4 jets (not 3,4 for single), mqq(double) and csv1
//Float_t MC_data[2] = {1./1.07,1./1.17};////////////trigger correction using 4 jets (not 3 for single), mqq(double) and csv1(double)
//Float_t MC_data[2] = {1./1.43,1./1.68};////////////trigger correction using 4 jets (not 3 for single), mqq(double) and csv1(double) and normalization with 3rd jet in double and 1st in single
//Float_t MC_data[2] = {1./1.06,1./1.46};////////////for efficiency
if (region_type!=0) {
	MC_data[0]=1.; 
	MC_data[1]=1.; 
}
Float_t lumi_qcd=lumi/MC_data[set_type];
do{
	TFile *file_initial;
  // if (files!=6) file_initial = new TFile(file_names[files]);
  	file_initial = TFile::Open(file_names[files]);
//	else  file_initial = new TFile("../../output_hist/v14/golden/skimmed_tree_analysis_doubleWJetsToLNu_v14_final_74cmssw.root");
	TH1F *histos[100];
	for (int hist=0;hist<nhistos;++hist){
		if (hist==73) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(4);
		if (hist==12) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(2.);
		histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("h");
		if (files==0) data_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("data");
		if ((files>0)&&(files<qcd_begin))histos[hist]->Scale(lumi);
		if (files>=qcd_begin)histos[hist]->Scale(lumi_qcd);
		
		if (files==1) 	sum_histos[hist] = (TH1F*)histos[hist]->Clone(sum_histos_names[hist]);
		if (files>1)	sum_histos[hist]->Add(histos[hist]); 

		if (files>0) histos[hist]->Sumw2(kFALSE);
		if (hist==4) cout<<files<<"   "<<histos[4]->Integral() <<endl;

		if (files==1) {
			signal_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"newhist");
			signal_histos[hist]->Scale(lumi);
			signal_histos[hist]->Sumw2(kFALSE);
			signal_histos[hist]->SetLineColor(LINECOLOR[files]);
			signal_histos[hist]->SetLineStyle(LINESTYLE[files]);
			signal_histos2[hist]=(TH1F*)signal_histos[hist]->Clone("signalHist2");
		}
		if (files==2) {
			gf_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"tthnb");
			gf_histos[hist]->Scale(lumi);
			gf_histos[hist]->Sumw2(kFALSE);
			gf_histos[hist]->SetLineColor(LINECOLOR[files]);
			gf_histos[hist]->SetLineStyle(LINESTYLE[files]);
		}
		if (files==3) {
			tthbb_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"tthb");
			tthbb_histos[hist]->Scale(lumi);
			tthbb_histos[hist]->Sumw2(kFALSE);
			tthbb_histos[hist]->SetLineColor(LINECOLOR[files]);
			tthbb_histos[hist]->SetLineStyle(LINESTYLE[files]);
		}
		if (files==4) {
			tthnbb_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"tthnb");
			tthnbb_histos[hist]->Scale(lumi);
			tthnbb_histos[hist]->Sumw2(kFALSE);
			tthnbb_histos[hist]->SetLineColor(LINECOLOR[files]);
			tthnbb_histos[hist]->SetLineStyle(LINESTYLE[files]);
		}
		histos[hist]->SetLineColor(LINECOLOR[files]);
		histos[hist]->SetLineStyle(LINESTYLE[files]);
		histos[hist]->SetLineWidth(LINEWIDTH[files]);
		histos[hist]->SetFillStyle(FILLSTYLE[files]);
		if ((files!=0)) histos[hist]->SetFillColor(FILLCOLOR[files]);
		if ((region_type!=0)&&(files>=qcd_begin)) histos[hist]->SetFillColor(kRed+2);
		
		if (files==0) {
			histos[hist]->SetMarkerStyle(20);
			data_histos[hist]->SetLineColor(1);
			data_histos[hist]->SetMarkerStyle(20);
			data_histos[hist]->SetMarkerSize(.8);
		}
	 	if (files>=bg_begin) stacks[hist]->Add(histos[hist]);
		if (hist==0) histos_for_legened[files] = (TH1F*)histos[0]->Clone("newd");
		if (files==bg_begin)	discr_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("discr");
		if (files>bg_begin)	discr_histos[hist]->Add(histos[hist]); 
   	}
		if (files>=bg_begin) totalBG+=histos[4]->Integral();
		if (files>=qcd_begin) totalQCD+=histos[4]->Integral();
		if (files>0) totalMC+=histos[4]->Integral();
		if (files==0) {totalData+=histos[4]->Integral(); totalDataQCD+=histos[4]->Integral();}
		if ((files>0)&&(files<qcd_begin)) {totalDataQCD-=histos[4]->Integral();}
		if (files==0) out_efficiency<<"Sample & \t\t\t yield(per "<< lumi<<" $pb^{-1}$)"<<endl;
		if (files==0) out_efficiency<<leg_names[order[files]]<<"&\t \t \t"<< std::setprecision(5)<<histos[4]->Integral() <<endl;
		else out_efficiency<<leg_names[order[files]]<<"&\t\t\t  "<<std::setprecision(5)<<histos[4]->Integral()<<endl;
		if (files==nfiles-1) out_efficiency<<"Total BG"<<"&\t \t \t  "<<std::setprecision(5)<<totalBG<<endl;
		if (files==nfiles-1) out_efficiency<<"Total MC"<<"&\t \t \t  "<<std::setprecision(5)<<totalMC<<endl;
		if (files==nfiles-1) out_efficiency<<"Data/MC"<<"&\t \t \t  "<<std::setprecision(3)<<totalData/totalMC<<endl;
		if (files==nfiles-1) out_efficiency<<"DataQCD/QCD"<<"&\t \t \t  "<<std::setprecision(3)<<totalDataQCD/totalQCD<<endl;
files++;
}while (files<nfiles);
out_efficiency.close();

for (int hist=0;hist<nhistos;hist++){
	hBkgUncUp[hist] = (TH1F*)sum_histos[hist]->Clone("hBkgUncUp");
	hBkgUncLo[hist] = (TH1F*)sum_histos[hist]->Clone("hBkgUncLo");
  	hBkgVis[hist]   = (TH1F*)sum_histos[hist]->Clone("hbkgVis");
  	for(int i=0;i<hBkgUncUp[hist]->GetNbinsX();i++) {
  		float e = 0.0;
    	if (sum_histos[hist]->GetBinContent(i+1) != 0) {
      	e = sum_histos[hist]->GetBinError(i+1)/sum_histos[hist]->GetBinContent(i+1);
   	}
    	hBkgUncUp[hist]->SetBinContent(i+1,e);
    	hBkgUncLo[hist]->SetBinContent(i+1,-e);
	}
  	hBkgVis[hist]->SetMarkerSize(0);
 	hBkgVis[hist]->SetFillColor(kBlack);
 	hBkgVis[hist]->SetFillStyle(3004);
 	hBkgUncUp[hist]->SetLineColor(kBlack);
 	hBkgUncUp[hist]->SetLineWidth(1);
 	hBkgUncUp[hist]->SetFillColor(kBlack);
 	hBkgUncUp[hist]->SetFillStyle(3004);
	hBkgUncLo[hist]->SetLineColor(kBlack);
 	hBkgUncLo[hist]->SetLineWidth(1);
  	hBkgUncLo[hist]->SetFillColor(kBlack);
	hBkgUncLo[hist]->SetFillStyle(3004);
}


//Float_t TSF[2] = {0.452433,0.525697}; old v13
Float_t TSF[2] = {0.737,0.69};
//Float_t TSF[2] = {1.,1.};
Float_t kfactors[2] = {0.,0.};
kfactors[set_type] = 1./(MC_data[set_type] * TSF[set_type]);
cout<<"kfactor = "<<kfactors[set_type]<<endl;
cout<<"Data/MC = "<< MC_data[set_type]<<endl;


for (int i=0;i<nfiles;i++){
	if (i==0) leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"P");
	if ((i==1)||(i==2)||(i==3) || (i==4))  leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"L");
	if ((i>=bg_begin)&&(i<qcd_begin)) leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"F");
}
if (region_type==0){
	for (int i=0;i<nfiles;i++){
		if (i>=qcd_begin) leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"F");
	}
}
if (region_type!=0) {
	leg->AddEntry(histos_for_legened[order_legend[qcd_begin]],"QCD","F");
}
leg->AddEntry(hBkgUncUp[0],"MC stat. unc.","F");
leg2->AddEntry(hBkgUncUp[0],"without dynamical","");
leg2->AddEntry(hBkgUncUp[0],"trigger SF","");



for (int d=0;d<nhistos;d++){
	discriminators[d] = Discr(discr_histos[d],signal_histos2[d]);
}

bubblesort(discriminators, hist_names_sort,nhistos);

out_discrimination.open(trigger[set_type]+dir_name+"discrimination.txt");
for (int d=0;d<nhistos;d++){
	if (d==0) out_discrimination<<"Variable &\t d"<<endl;
	out_discrimination<<"$"<<hist_names_sort[d]<<"$"<<" & \t "<< std::setprecision(2)<< discriminators[d]<<endl;
}
out_discrimination.close();

TLatex* tex = new TLatex(0.75,0.95,"2.19 fb^{-1} (13 TeV)");
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
TLatex* tex2 = new TLatex(0.27,0.89,"Work in progress");
tex2->SetNDC();
tex2->SetTextAlign(20);
tex2->SetTextFont(52);
tex2->SetTextSize(0.035);
tex2->SetLineWidth(2);
TString temp_str;
temp_str.Form("%2.2f",kfactors[set_type]);
//		temp_str.Form("%2.2f",(1./MC_data[set_type]));
TString k_factor_str = temp_str.Prepend("QCD MC #times  ");
TLatex* tex_k = new TLatex(0.63,0.89,k_factor_str);
tex_k->SetNDC();
tex_k->SetTextAlign(20);
tex_k->SetTextFont(42);
tex_k->SetTextSize(0.03);
tex_k->SetLineWidth(2);
//for (int i=0;i<nhistos;i++){
	for (int i=69;i<nhistos;i++){
		//temp_str.Form("%2.2f",Discr(discr_histos[i],signal_histos2[i]));
		Float_t d_value = Discr((TH1F*)data_histos[i]->Clone("data2"),signal_histos2[i]);
		temp_str.Form("%2.2f",d_value);
		TString disc_value = temp_str.Prepend(" d = ");
		TLatex *disc_value_text = new TLatex(0.668,0.86,disc_value);
      disc_value_text->SetNDC();
     	disc_value_text->SetTextAlign(20);
      disc_value_text->SetTextFont(42);
      disc_value_text->SetTextSize(0.03);
      disc_value_text->SetLineWidth(2);
		
		temp_str.Form("%2d",i);
		TString can_name="c1";
		can_name.Append(temp_str);
		TCanvas *c1 = new TCanvas(can_name,"",900,750);
		c1->cd();
		gPad->SetLogy(0);
		c1->SetBottomMargin(.3);
		c1->SetRightMargin(.25);
	
		bool LOGY=true;
		if ((region_type==0)&&(i==51)) LOGY=false;	
	//	bool LOGY=std::find(LOGY_array.begin(),LOGY_array.end(),i) != LOGY_array.end();
  	//	if ((region_type==0) && (LOGY==true)) gPad->SetLogy();
  		if ((region_type==0) && (LOGY==true)) gPad->SetLogy();

		Double_t xmin = signal_histos[i]->GetBinCenter(0);
		Double_t xmax = signal_histos[i]->GetBinCenter(signal_histos[i]->GetNbinsX())*1.02;
		if ((region_type!=0 ) && (i==66)) {xmin=0.;xmax=400;}
		if ((region_type!=0 ) &&( (i==71)||(i==72))) {xmin=0.;xmax=0.05;}
		TH1F *frame = new TH1F("frame","",1,xmin,xmax);
		frame->Reset();
		frame->SetMinimum(1e-3);
      frame->SetMaximum(1e8);
		if ((region_type!=0) || (LOGY==false))	frame->SetMaximum(data_histos[i]->GetMaximum()*1.2);
      frame->GetXaxis()->SetTitleOffset(0.91);
      frame->SetStats(0);
		frame->GetYaxis()->SetNdivisions(505);
	 	frame->GetXaxis()->SetLabelSize(0.0);
		char name[1000];
		if (UNITS[i]==0) {
			if (data_histos[i]->GetBinWidth(1)>1) sprintf(name,"Events / %1.0f",data_histos[i]->GetBinWidth(1));
			else sprintf(name,"Events / %1.2f",data_histos[i]->GetBinWidth(1));
		} else {
      	sprintf(name,"Events / %1.0f %s",data_histos[i]->GetBinWidth(1),"GeV");
		}
		frame->GetYaxis()->SetTitle(name);

      frame->Draw();
		tex->Draw();
		tex1->Draw();
		tex2->Draw();
		if (region_type==0) tex_k->Draw();
		if ((region_type==0)&&(d_value>0.1)&&(d_value<1.)) disc_value_text->Draw();
    	stacks[i]->Draw("same");	
		signal_histos[i]->Draw("same");
		tthbb_histos[i]->Draw("same");
		tthnbb_histos[i]->Draw("same");
		gf_histos[i]->Draw("same");
/////////////////cross check of stupid THStack//////
//
//		sum_histos[i]->SetLineColor(kCyan);
//		sum_histos[i]->Scale(lumi);
//		sum_histos[i]->Draw("Lsame");
//
///////////////////////////////////////////////////
		data_histos[i]->Draw("Psame");
		hBkgVis[i]->Draw("same E2");

		leg->Draw("same");
		leg2->Draw("same");
  		gPad->RedrawAxis();
	
  		TPad* pad2 = new TPad("pad2", "pad2", 0., 0., 1., 1.);
 		pad2->SetTopMargin(0.73);
 		pad2->SetRightMargin(0.25);
 		pad2->SetFillColor(0);
  		pad2->SetFillStyle(0);
  		pad2->Draw();
  		pad2->cd(0);
  		gPad->SetGridy();

		TH1F *frame2 = new TH1F("frame2","",1,xmin,xmax);
		frame2->SetMinimum(-1);
      frame2->SetMaximum(1);
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
		frame2->SetXTitle(signal_histos[i]->GetXaxis()->GetTitle());
		frame2->SetYTitle("Data / MC - 1");
		frame2->Draw();	

 		Double_t aa[2] = {xmin,xmax};
   	Double_t bb[2] = {0,0};
   	TGraph *cons = new TGraph(2,aa,bb);
    	cons->SetLineStyle(2);
		cons->Draw("Lsame");

		data_histos2[i] = (TH1F*)data_histos[i]->Clone("new");
		data_histos2[i]->Add(sum_histos[i],-1);
		data_histos2[i]->Divide(sum_histos[i]);
		data_histos2[i]->Draw("PEsame");
		hBkgUncUp[i]->Draw("HIST same");
		hBkgUncLo[i]->Draw("HIST same");
		pad2->RedrawAxis();
		c1->Print(output_names[i]);
		c1->Delete();
	}



return 0;
}
