#include <ctype.h>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TTree.h"
#include "TChain.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
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
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_double.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/preselection_single_blike.C"
#include "/afs/cern.ch/work/n/nchernya/Hbb/trigger_maps3.C"


//Double_t erf( Double_t *x, Double_t *par){
//  return par[0]/2.*(1.+TMath::Erf((x[0]-par[1])/par[2]));
//}
#define SWAP2(A, B) { TLorentzVector t = A; A = B; B = t; }
void SortByEta(std::vector<TLorentzVector> &jets){
  int i, j;
	int n=jets.size();
  for (i = n - 1; i >= 0; i--){
    for (j = 0; j < i; j++){
      if (jets[j].Eta() < jets[j + 1].Eta() ){
        SWAP2( jets[j], jets[j + 1] );
		}
    }
	}
}

typedef std::map<double, int> JetList;
const int njets = 300;

typedef struct {
   Float_t eta[njets];
   Float_t pt[njets];
   Float_t phi[njets];
	Float_t mass[njets];
	Float_t btag[njets];
	Int_t nsoft;
	Float_t soft_pt[njets];
	Float_t soft_eta[njets];
	Float_t soft_mass[njets];
	Float_t qgl[njets];
	Int_t nsoft2;
	Int_t nsoft5;
	Int_t nsoft10;
	Int_t id[njets];
	Int_t puId[njets];
	Float_t HTsoft;
	Float_t pt_regVBF[njets];	
	Float_t ptd[njets];
	Float_t axis2[njets];
	Int_t mult[njets];
	Float_t blike_VBF[njets];
	Float_t qgl1_VBF[njets];
	Float_t qgl2_VBF[njets];
} Jets;

using namespace std;


///////    0 18 0/1 0/1/2

int main(int argc, char* argv[]){

int files=atoi(argv[1]);
int files_max=atoi(argv[2]);
int set_type=atoi(argv[3]);
int region_type=atoi(argv[4]); // 0 - analysis, 1 - control region , top
TString region[3]={"_analysis","_controlTop","_controlDY"}; // 0 - analysis, 1 - control region , top; 2 - control region DY

TString dataset_type[2] = {"","_single"};
TString file_postfix[2] = {"_v14","_v14"};

const int nfiles  = 20;

TString file_names[nfiles] = {"QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500","QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf", "VBFHToBB_M-125_13TeV_powheg", "GF", "BTagCSV","TTbar","DYtoQQ","ST_tW","ttHtobb","ttHtoNbb", "DYtoLL"};
Double_t xsec[nfiles] = { 2.75E07, 1.74E06,  3.67E05, 2.94E04, 6.52E03,1.064E03,   121.5,  2.54E01,  2.16 , 25.17 ,1.,816.,1461., 71.7,0.2934,  0.2151,  6025.2  };
//Float_t BG[nfiles] = {1.,1.,1.,1.,1.,1.,1.,1,1.,1.,0.,1.,1.,1.,1.,1.,1.}; //it is actually all MC not only BG
Float_t BG[nfiles] =   {0.,0.,0, 0, 0, 0, 0, 0,0 ,0 ,0.,0.,0.,0.,0.,0.,0.}; //it is actually all MC not only BG
Float_t data[nfiles]={0.,0.,0.,0.,0.,0.,0.,0,0.,0.,1.,0.,0.,0.,0.,0.,0.};
     
TString type[nfiles]; 		
for (int i=0;i<nfiles;i++){
	type[i] = file_names[i];
}


do {
	
	Float_t presel=0;
	Float_t presel_vtype[10] = {0,0,0,0,0,0,0,0,0};
	Float_t pos_puweight=0;
	Float_t all_puweight=0.;
	Float_t puweight;
	Float_t PU=1.;
	Float_t genweight;
	Float_t genweight0;
	Int_t global_counter = 0;
	Float_t HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200;
	Float_t HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460;
	TFile *file_initial;
	TChain *tree_initial;

//	TString path = "/shome/nchernya/Hbb/skim_trees/v14/";
//	file_initial = TFile::Open(path+file_names[files]+"_v14"+dataset_type[set_type]+"/"+file_names[files]+file_postfix[set_type]+dataset_type[set_type]+".root");

/////////////////qgd//////////////
	dataset_type[0] = "_double";
	dataset_type[1] = "_single";
	TString path ;
	path= "dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v14/qgd/";
	if (set_type==0)	file_initial = TFile::Open(path+file_names[files]+dataset_type[set_type]+".root");
	if (set_type==1) file_initial = TFile::Open(path+"qgd_"+file_names[files]+dataset_type[set_type]+"_2jets"+".root");
////////////////////////////

	cout<<files<<endl;
	
	tree_initial = (TChain*)file_initial->Get("tree");
	Int_t events_generated;
	TH1F *countPos;
	TH1F *countNeg;
	if ((data[files]!=1)){
		countPos = (TH1F*)file_initial->Get("CountPosWeight");
 		countNeg = (TH1F*)file_initial->Get("CountNegWeight");
 		events_generated = countPos->GetEntries()-countNeg->GetEntries();
	} else events_generated = 1;
    Jets Jet;
    Float_t v_type;
    Float_t wrong_type=0.;
    Int_t nJets;
	Float_t JSON;	
	Float_t nPVs;	

	Float_t met_pt;

	Jets GenHiggsSisters;

	int pos_weight_presel=0;
 	Int_t selLeptons_tightId[20];
	Float_t selLeptons_relIso03[20] , selLeptons_chargedHadRelIso03[20], selLeptons_pfRelIso03[20];


    tree_initial->SetBranchAddress("Vtype",&v_type);
    tree_initial->SetBranchAddress("nJet",&nJets);
    tree_initial->SetBranchAddress("Jet_pt",Jet.pt);
    tree_initial->SetBranchAddress("Jet_eta",Jet.eta);
    tree_initial->SetBranchAddress("Jet_phi",Jet.phi);
	tree_initial->SetBranchAddress("Jet_mass",Jet.mass);
	tree_initial->SetBranchAddress("Jet_btagCSV",Jet.btag);
	tree_initial->SetBranchAddress("Jet_blike_VBF",Jet.blike_VBF);
	tree_initial->SetBranchAddress("Jet_id",Jet.id);	
	tree_initial->SetBranchAddress("Jet_puId",Jet.puId);
	tree_initial->SetBranchAddress("met_pt",&met_pt);
	
	tree_initial->SetBranchAddress("softActivityJets_pt",Jet.soft_pt);
	tree_initial->SetBranchAddress("softActivityJets_eta",Jet.soft_eta);
	tree_initial->SetBranchAddress("softActivityJets_mass",Jet.soft_mass);
	tree_initial->SetBranchAddress("softActivity_HT",&Jet.HTsoft);
	tree_initial->SetBranchAddress("softActivity_njets2",&Jet.nsoft2);
	tree_initial->SetBranchAddress("softActivity_njets5",&Jet.nsoft5);
	tree_initial->SetBranchAddress("softActivity_njets10",&Jet.nsoft10);
	tree_initial->SetBranchAddress("Jet_qgl",Jet.qgl);
	tree_initial->SetBranchAddress("genWeight",&genweight);
	tree_initial->SetBranchAddress("puWeight",&puweight);
	tree_initial->SetBranchAddress("nPVs",&nPVs);
	tree_initial->SetBranchAddress("Jet_ptd",Jet.ptd);
	tree_initial->SetBranchAddress("Jet_axis2",Jet.axis2);
	tree_initial->SetBranchAddress("Jet_mult",Jet.mult);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v",&HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200);
 	tree_initial->SetBranchAddress("HLT_BIT_HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v",&HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460);
	tree_initial->SetBranchAddress("Jet_pt_regVBF",Jet.pt_regVBF);
	tree_initial->SetBranchAddress("json",&JSON);
    
	tree_initial->SetBranchAddress("GenHiggsSisters_pt",GenHiggsSisters.pt);
    tree_initial->SetBranchAddress("GenHiggsSisters_eta",GenHiggsSisters.eta);
    tree_initial->SetBranchAddress("GenHiggsSisters_phi",GenHiggsSisters.phi);
	tree_initial->SetBranchAddress("GenHiggsSisters_mass",GenHiggsSisters.mass);

	tree_initial->SetBranchAddress("selLeptons_tightId",selLeptons_tightId);
	tree_initial->SetBranchAddress("selLeptons_relIso03",selLeptons_relIso03);
	tree_initial->SetBranchAddress("selLeptons_chargedHadRelIso03",selLeptons_chargedHadRelIso03);
	tree_initial->SetBranchAddress("selLeptons_pfRelIso03",selLeptons_pfRelIso03);

	tree_initial->SetBranchAddress("Jet_qgl1_VBF",Jet.qgl1_VBF);
	tree_initial->SetBranchAddress("Jet_qgl2_VBF",Jet.qgl2_VBF);




	if (data[files]==1){
		genweight = 1.;
		puweight=1.;
	}

 	
    TH1F *hJet1_pt_bin = new TH1F("hJet1_pt_bin", "", 50, 90., 140.);
    hJet1_pt_bin->GetXaxis()->SetTitle("1st Jet p_{T} (GeV)");
    TH1F *hJet2_pt_bin = new TH1F("hJet2_pt_bin", "", 40, 70., 110.);
    hJet2_pt_bin->GetXaxis()->SetTitle("2nd Jet p_{T} (GeV)");
    TH1F *hJet3_pt_bin = new TH1F("hJet3_pt_bin", "", 30, 60., 90.);
    hJet3_pt_bin->GetXaxis()->SetTitle("3rd Jet p_{T} (GeV)");
    TH1F *hJet4_pt_bin = new TH1F("hJet4_pt_bin", "", 40, 30., 70.);
    hJet4_pt_bin->GetXaxis()->SetTitle("4th Jet p_{T} (GeV)");
	TH1F *hMqq_bin = new TH1F("hMqq_bin","",50.,200.,250.);
	hMqq_bin->GetXaxis()->SetTitle("m_{qq} (GeV)");
   
    TH1F *hJet1_pt = new TH1F("hJet1_pt", "", 30, 0., 600.);
    hJet1_pt->GetXaxis()->SetTitle("1st Jet p_{T} (GeV)");
    TH1F *hJet2_pt = new TH1F("hJet2_pt", "", 30, 0., 600.);
    hJet2_pt->GetXaxis()->SetTitle("2nd Jet p_{T} (GeV)");
    TH1F *hJet3_pt = new TH1F("hJet3_pt", "", 20, 0., 400.);
    hJet3_pt->GetXaxis()->SetTitle("3rd Jet p_{T} (GeV)");
    TH1F *hJet4_pt = new TH1F("hJet4_pt", "", 20, 0., 400.);
    hJet4_pt->GetXaxis()->SetTitle("4th Jet p_{T} (GeV)");
    
    TH1F *hJet5_pt = new TH1F("hJet5_pt", "", 25, 0., 500.);
    hJet5_pt->GetXaxis()->SetTitle("5th Jet p_{T} (GeV)");
    
	TH1F *hJet1_eta = new TH1F("hJet1_eta", "", 20, -5., 5.);
    hJet1_eta->GetXaxis()->SetTitle("1st Jet #eta");
    TH1F *hJet2_eta = new TH1F("hJet2_eta", "", 20, -5., 5.);
    hJet2_eta->GetXaxis()->SetTitle("2nd Jet #eta");
    TH1F *hJet3_eta = new TH1F("hJet3_eta", "", 20, -5., 5.);
    hJet3_eta->GetXaxis()->SetTitle("3rd Jet #eta");
    TH1F *hJet4_eta = new TH1F("hJet4_eta", "", 20, -5., 5.);
    hJet4_eta->GetXaxis()->SetTitle("4th Jet #eta");
    
    TH1F *hJet1_phi = new TH1F("hJet1_phi", "", 32,-3.2,3.2);
    hJet1_phi->GetXaxis()->SetTitle("1st Jet #phi");
    TH1F *hJet2_phi = new TH1F("hJet2_phi", "", 32,-3.2,3.2);
    hJet2_phi->GetXaxis()->SetTitle("2nd Jet #phi");
    TH1F *hJet3_phi = new TH1F("hJet3_phi", "", 32,-3.2,3.2);
    hJet3_phi->GetXaxis()->SetTitle("3rd Jet #phi");
    TH1F *hJet4_phi = new TH1F("hJet4_phi", "", 32,-3.2,3.2);
    hJet4_phi->GetXaxis()->SetTitle("4th Jet #phi");
    TH1F *hVtype = new TH1F("hVtype","", 6,-1.,6.);
    hVtype->GetXaxis()->SetTitle("vtype");

	TH1F *hMqq = new TH1F("hMqq","",100.,0.,3000.);
	hMqq->GetXaxis()->SetTitle("m_{qq} (GeV)");
   
	TH1F *hEtaQQ = new TH1F("hEtaQQ","",80,0.,8.);
	hEtaQQ->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");

	TH1F *hPhiQQ = new TH1F("hPhiQQ","",32,0.,3.2);
	hPhiQQ->GetXaxis()->SetTitle("|#Delta#phi_{qq}|");
    
	TH1F *hPhiBB = new TH1F("hPhiBB","",32,0.,3.2);
	hPhiBB->GetXaxis()->SetTitle("|#Delta#phi_{bb}|");
	
	TH1F *hEtaSoftJets = new TH1F("hEtaSoftJets","",12,-3.,3.);
	hEtaSoftJets->GetXaxis()->SetTitle("|#eta^{soft}|");
    
	
	TH1F *hMassSoftJets = new TH1F("hMassSoftJets","",10,0.,100.);
	hMassSoftJets->GetXaxis()->SetTitle("m^{soft}");

	TH1F *hHTsoft = new TH1F("hHTsoft","",60,0.,600.);
	hHTsoft->GetXaxis()->SetTitle("H_{T}^{soft} (GeV)" );
	TH1F *hSoft_n2 = new TH1F("hSoft_n2","",25,0.,25.);
	hSoft_n2->GetXaxis()->SetTitle("Multiplicity of soft jets with p_{T} > 2 GeV");
	TH1F *hSoft_n5 = new TH1F("hSoft_n5","",10,0.,10.);
	hSoft_n5->GetXaxis()->SetTitle("Multiplicity of soft jets with p_{T} > 5 GeV");
	TH1F *hSoft_n10 = new TH1F("hSoft_n10","",6,0.,6.);
	hSoft_n10->GetXaxis()->SetTitle("Multiplicity of soft jets with p_{T} > 10 GeV");
	
	TH1F* hMbb = new TH1F("hMbb","",50,0.,250.);
	hMbb->GetXaxis()->SetTitle("m_{bb} (GeV)");
	TH1F* hqgl = new TH1F("hqgl","",20.,0.,1.);
	hqgl->GetXaxis()->SetTitle("QGL of the first quark jet candidate");
	
	TH1F* hqgl2 = new TH1F("hqgl2","",20.,0.,1.);
	hqgl2->GetXaxis()->SetTitle("QGL of the second quark jet candidate");

	TH1F *hbtag = new TH1F("hbtag","",110,0.,1.1);
	hbtag->GetXaxis()->SetTitle("CSV 1^{st} b-jet");
 
	TH1F *hbtag2 = new TH1F("hbtag2","",110,0.,1.1);
	hbtag2->GetXaxis()->SetTitle("CSV 2^{nd} b-jet");
	
	TH1F *hPtSoftJets = new TH1F("hPtSoftJets","",30,0.,300);
	hPtSoftJets->GetXaxis()->SetTitle("p_{T}^{soft} (GeV)");
    TH1F *hPtSoftJets2 = new TH1F("hPtSoftJets2", "", 20, 0., 200.);
    hPtSoftJets2->GetXaxis()->SetTitle("2nd Soft Jet p_{T} (GeV)");
    TH1F *hPtSoftJets3 = new TH1F("hPtSoftJets3", "", 20, 0., 200.);
    hPtSoftJets3->GetXaxis()->SetTitle("3rd Soft Jet p_{T} (GeV)");
	
	TH1F *hcosOqqbb = new TH1F("hcosOqqbb","",100,-1.,1.);
	hcosOqqbb->GetXaxis()->SetTitle("cos(#theta_{bb_qq})");
	TH1F *hEtaQB1 = new TH1F("hEtaQB1","",160.,-8.,8.);
	hEtaQB1->GetXaxis()->SetTitle("#Delta#eta_{qb}^{forward}");
	TH1F *hEtaQB2 = new TH1F("hEtaQB2","",160.,-8.,8.);
	hEtaQB2->GetXaxis()->SetTitle("#Delta#eta_{qb}^{backward}");
	TH1F *hPhiQB1 = new TH1F("hPhiQB1","",32,0.,3.2);
	hPhiQB1->GetXaxis()->SetTitle("#Delta#phi_{qb}^{forward}");
	TH1F *hPhiQB2 = new TH1F("hPhiQB2","",32,0.,3.2);
	hPhiQB2->GetXaxis()->SetTitle("#Delta#phi_{qb}^{backward}");
	TH1F *hx1 = new TH1F("hx1","",100.,0.,1.);
	hx1->GetXaxis()->SetTitle("x_{1}");
	TH1F *hx2 = new TH1F("hx2","",100.,0.,1.);
	hx2->GetXaxis()->SetTitle("x_{2}");
	TH1F *hVB1_mass = new TH1F("hVB1_mass","",100,0.,1000.);
	hVB1_mass->GetXaxis()->SetTitle("M_{W'_{1}} (GeV)");
	TH1F *hVB2_mass = new TH1F("hVB2_mass","",100.,0.,1000.);
	hVB2_mass->GetXaxis()->SetTitle("M_{W'_{2}} (GeV)");

	TH1F* hEtot = new TH1F("hEtot","",150.,0.,6000.);
	hEtot->GetXaxis()->SetTitle("E^{tot} (GeV)");
	TH1F* hPxtot= new TH1F("hPxtot","",100,-500.,500.);
	hPxtot->GetXaxis()->SetTitle("P_{x}^{tot} (GeV)");
	TH1F* hPytot= new TH1F("hPytot","",100,-500.,500.);
	hPytot->GetXaxis()->SetTitle("P_{y}^{tot} (GeV)");
	TH1F* hPztot= new TH1F("hPztot","",100,-5000.,5000);
	hPztot->GetXaxis()->SetTitle("P_{z}^{tot} (GeV)");

	
	TH1F *hPtqqbb = new TH1F("hPtqqbb","",50.,0.,500.);
	hPtqqbb->GetXaxis()->SetTitle("p_{T} of qqbb system (GeV)");
	TH1F *hPhiqqbb = new TH1F("hPhiqqbb","",32,-3.2,3.2);
	hPhiqqbb->GetXaxis()->SetTitle("-#phi of qqbb system");
	TH1F *hEtaqqbb = new TH1F("hEtaqqbb","",160,-8,8);
	hEtaqqbb->GetXaxis()->SetTitle("#eta of qqbb system");

	TH1F *hnPVs = new TH1F("hPVs","",50,0,50);
	hnPVs->GetXaxis()->SetTitle("nPVs");
	
	TH1F* hMbb_regVBF = new TH1F("hMbb_regVBF","",50,0.,250.);
	hMbb_regVBF->GetXaxis()->SetTitle("m_{bb} after VBF regression (GeV)");

	TH1F* hMbb_regVBF_fsr = new TH1F("hMbb_regVBF_fsr","",50,0.,250.);
	hMbb_regVBF_fsr->GetXaxis()->SetTitle("m_{bb} after VBF regression + FSR jet (GeV)");

	TH1F *hJet1q_pt = new TH1F("hJet1q_pt","",17,30,200.);
	hJet1q_pt->GetXaxis()->SetTitle("p_{T} of first quark jet candidate");
	TH1F *hJet1q_eta = new TH1F("hJet1q_eta","",20,-5,5);
	hJet1q_eta->GetXaxis()->SetTitle("#eta of first quark jet candidate");
	TH1F *hJet1q_ptd = new TH1F("hJet1q_ptd","",100,0,1);
	hJet1q_ptd->GetXaxis()->SetTitle("ptd of first quark jet candidate");
	TH1F *hJet1q_axis2= new TH1F("hJet1q_axis2","",120,-2,10.);
	hJet1q_axis2->GetXaxis()->SetTitle("axis2 of first quark jet candidate");
	TH1F *hJet1q_mult= new TH1F("hJet1q_mult","",30,0,30.);
	hJet1q_mult->GetXaxis()->SetTitle("Multiplicity of first quark jet candidate");
	TH1F *hJet2q_pt = new TH1F("hJet2q_pt","",17,30,200);
	hJet2q_pt->GetXaxis()->SetTitle("p_{T} of second quark jet candidate");
	TH1F *hJet2q_eta = new TH1F("hJet2q_eta","",20,-5,5);
	hJet2q_eta->GetXaxis()->SetTitle("#eta of second quark jet candidate");
	TH1F *hJet2q_ptd = new TH1F("hJet2q_ptd","",100,0,1);
	hJet2q_ptd->GetXaxis()->SetTitle("ptd of second quark jet candidate");
	TH1F *hJet2q_axis2= new TH1F("hJet2q_axis2","",120,-2,10.);
	hJet2q_axis2->GetXaxis()->SetTitle("axis2 of second quark jet candidate");
	TH1F *hJet2q_mult= new TH1F("hJet2q_mult","",30,0,30.);
	hJet2q_mult->GetXaxis()->SetTitle("Multiplicity of second quark jet candidate");

	TH1F *hblike1 = new TH1F("hblike1","",100,-1,1);
	hblike1->GetXaxis()->SetTitle("b-likelihood of 1^{st} b-jet");
	TH1F *hblike2 = new TH1F("hblike2","",100,-1,1);
	hblike2->GetXaxis()->SetTitle("b-likelihood of 2^{nd} b-jet");

	TH1F *hmet = new TH1F("hmet","",100,0.,1000.);
	hmet->GetXaxis()->SetTitle("MET p_{T} (GeV)");


	TH1F *hselLeptons_tightId = new TH1F("hselLeptons_tightId","",8,-2.5,5.5 );
	hselLeptons_tightId->GetXaxis()->SetTitle("selLeptons_tightId[0]");
	TH1F *hselLeptons_relIso03= new TH1F("hselLeptons_relIso03","",50,-5.5,-0.5 );
	hselLeptons_relIso03->GetXaxis()->SetTitle("selLeptons_relIso03[0]");
	TH1F *hselLeptons_chargedHadRelIso03 = new TH1F("hselLeptons_chargedHadRelIso03","",50,-5.5,-0.5 );
	hselLeptons_chargedHadRelIso03->GetXaxis()->SetTitle("selLeptons_chargedHadRelIso03[0]");
	TH1F *hselLeptons_pfRelIso03= new TH1F("hselLeptons_pfRelIso03","",50,-5.5,-0.5 );
	hselLeptons_pfRelIso03->GetXaxis()->SetTitle("selLeptons_pfRelIso03[0]");

	TH1F *hqgl1_VBF = new TH1F("hqgl1_VBF","",50,0,1);
	hqgl1_VBF->GetXaxis()->SetTitle("VBF QGD of the first quark jet candidate");
	TH1F *hqgl2_VBF = new TH1F("hqgl2_VBF","",50,0,1);
	hqgl2_VBF->GetXaxis()->SetTitle("VBF QGD of the second quark jet candidate");



   		const int numArray= 74; //54 without qgl stuff
   		TH1F* histArray[numArray] = {hJet1_pt,hJet2_pt,hJet3_pt,hJet4_pt,  hJet1_eta,hJet2_eta,hJet3_eta,hJet4_eta,  hJet1_phi,hJet2_phi,hJet3_phi,hJet4_phi, hMqq, hEtaQQ, hPhiBB, hEtaSoftJets, hPtSoftJets,hMassSoftJets,hHTsoft,hSoft_n2,hSoft_n5,hSoft_n10,hMbb,hqgl,hbtag,hqgl2,hbtag2,hPtSoftJets2,hPtSoftJets3,hcosOqqbb,hEtaQB1, hEtaQB2, hPhiQB1, hPhiQB2,hx1,hx2,hVB1_mass, hVB2_mass, hEtot, hPxtot, hPytot, hPztot, hJet5_pt,hPtqqbb, hPhiqqbb, hEtaqqbb, hJet1_pt_bin,hJet2_pt_bin,hJet3_pt_bin,hJet4_pt_bin, hMqq_bin, hnPVs, hMbb_regVBF, hMbb_regVBF_fsr, hJet1q_pt, hJet1q_eta, hJet1q_ptd, hJet1q_axis2, hJet1q_mult, hJet2q_pt, hJet2q_eta, hJet2q_ptd, hJet2q_axis2, hJet2q_mult,hblike1,hblike2, hmet,  hselLeptons_tightId , hselLeptons_relIso03 , hselLeptons_chargedHadRelIso03, hselLeptons_pfRelIso03, hqgl1_VBF,hqgl2_VBF, hPhiQQ };
			for (int i=0;i<numArray;i++){
				histArray[i]->Sumw2();
			}

	
	float qq_matching = 0;
	float qq_matching_all = 0;
	

	int nentries = tree_initial->GetEntries() ;

    for (int entry=0; entry<nentries;++entry){
        tree_initial->GetEntry(entry);

		if (JSON!=1) {
			continue;
		}

	
		
		if (region_type==0) {
			if (!((v_type==-1)||(v_type==4))) continue;
	
		} 
		else if (region_type==1) {
			if (!((v_type==2)||(v_type==3))) continue;
			if (met_pt<50) continue;
			if (!(selLeptons_relIso03[0]<0.08)) continue;
			if (!(selLeptons_chargedHadRelIso03[0]<0.05)) continue;
		}
		else if (region_type==2) {
			if (!((v_type==0)||(v_type==1))) continue;
			if (!((selLeptons_tightId[0]==1)||(selLeptons_tightId[0]==3))) continue;
			if (!((selLeptons_tightId[1]==1)||(selLeptons_tightId[1]==3))) continue;
		}
		

		if (data[files]==1) PU=1.;
		else PU=puweight;
		genweight0 = TMath::Sign(1.,genweight)*PU;
		genweight=TMath::Sign(1.,genweight)*PU;
		genweight/=events_generated/xsec[files]; 



		int btag_max1_number = -1;
		int btag_max2_number = -1;
		int pt_max1_number = -1;
		int pt_max2_number = -1;
		TLorentzVector Bjet1;
		TLorentzVector Bjet2;
		TLorentzVector Qjet1;
		TLorentzVector Qjet2;
		TLorentzVector qq;
		
		int flag=0;


///////// preselection(Int_t nJets, Float_t Jet_pt[300], Float_t Jet_eta[300], Float_t Jet_phi[300], Float_t Jet_mass[300], Float_t Jet_btagCSV[300], Int_t id[300], Int_t& btag_max1_number, Int_t& btag_max2_number, Int_t& pt_max1_number, Int_t& pt_max2_number, Float_t trigger, TLorentzVector& Bjet1,TLorentzVector& Bjet2, TLorentzVector& Qjet1, TLorentzVector& Qjet2,TLorentzVector& qq, Float_t scale=1.)
		if (set_type==0) {
			if (BG[files]==1) {
		//		preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq);
      ////////////////////////////////////////// 
	//			if (preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue; 	
	//			if (preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq)!=0) continue;
		/////////////////////////////////////////
				if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue; 	
				if (preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq)!=0) continue;
		/////////////////////////////////////////
			}
			if (BG[files]==0){
		//		preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq); 
			/////////////////////////////////////////
		//		if (preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue; 	
		//		if (preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq)!=0 )continue;
			////////////////////////////////////////	
				if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq) == 0) continue; 	
				if (preselection_double(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq)!=0) continue;
		/////////////////////////////////////////
				if (HLT_QuadPFJet_DoubleBTag_CSV_VBF_Mqq200!=1) continue;	
			}
		}
		else if (set_type==1){
			if (BG[files]==1) {
	//			preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq); 
			////////////////////////////////////////		
				//if (preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue; 
			////////////////////////////////////////
		if 	(preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq)!=0) continue;
			////////////////////////////////////////

			}

			if (BG[files] == 0) { 
//			preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq); 
			////////////////////////////////////////	
//				if (preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq) != 0) continue;
			////////////////////////////////////////	
			if (preselection_single_blike(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.blike_VBF, Jet.id, btag_max1_number, btag_max2_number, pt_max1_number, pt_max2_number, 1., Bjet1, Bjet2, Qjet1, Qjet2, qq)!=0) continue;
			////////////////////////////////////////	
				if (HLT_QuadPFJet_SingleBTag_CSV_VBF_Mqq460!=1) continue;	
			}
		}
	


	

		int matched_qq = 0;
		for (int i=0;i<nJets;i++){
			if (!(((Jet.id[i]>0)||(TMath::Abs(Jet.eta[i])>3))&& (Jet.pt[i]>20) )) continue;
		//	if (!(/*(Jet.id[i]>0)&&*/(Jet.pt[i]>20))) continue;
			TLorentzVector hJ0;
		   hJ0.SetPtEtaPhiM(Jet.pt[i],Jet.eta[i],Jet.phi[i],Jet.mass[i]);	
		   for(int j =0; j<2; j++){
				TLorentzVector hQ;
				hQ.SetPtEtaPhiM(GenHiggsSisters.pt[j],GenHiggsSisters.eta[j],GenHiggsSisters.phi[j],GenHiggsSisters.mass[j]);
				if(hQ.DeltaR(hJ0)<0.8) matched_qq++;
		   }	
		} 
		if (matched_qq>=2) qq_matching++;
		qq_matching_all++;	



		Float_t Mqq = qq.M();
		Float_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Float_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		Float_t qqDeltaPhi = TMath::Abs(Qjet1.DeltaPhi(Qjet2));


////////////////////////////

		vector<TLorentzVector> jets;
		vector<Float_t> jets_btag;
		for (int i=0;i<4;i++){
			TLorentzVector jet0;
			jet0.SetPtEtaPhiM(Jet.pt[i],Jet.eta[i],Jet.phi[i],Jet.mass[i]);
			jets.push_back(jet0);
		}
		SortByEta(jets);
		TLorentzVector Qjet1_eta = jets[0];	
		TLorentzVector Qjet2_eta = jets[3];
		jets.clear();
		Float_t Mqq_eta=(Qjet1_eta+Qjet2_eta).M();	
		Float_t Detaqq_eta = TMath::Abs(Qjet1_eta.Eta()-Qjet2_eta.Eta());
		for (int i=0;i<nJets;i++){
			if (Jet.pt[i]>30)	jets_btag.push_back(Jet.btag[i]);
		}
		std::sort(jets_btag.begin(), jets_btag.end()); 
		std::reverse(jets_btag.begin(), jets_btag.end()); 
		Float_t CSV1;
		Float_t CSV2;
		if (jets_btag[0]>1.) CSV1 = 1.;
		if (jets_btag[0]<0.) CSV1 = 0.;
		else CSV1=jets_btag[0];
		if (jets_btag[1]>1.) CSV2 = 1.;
		if (jets_btag[1]<0.) CSV2 = 0.;
		else CSV2=jets_btag[1];
		
		if ((BG[files]==1)&&(set_type==1)) {
//////////////////Single Preselection following trigger logic:
		int btag_max1_number_tr = -1;
		int btag_max2_number_tr = -1;
		int pt_max1_number_tr = -1;
		int pt_max2_number_tr = -1;
		TLorentzVector Bjet1_tr;
		TLorentzVector Bjet2_tr;
		TLorentzVector Qjet1_tr;
		TLorentzVector Qjet2_tr;
		TLorentzVector qq_tr;
		preselection_single(nJets, Jet.pt,Jet.eta, Jet.phi, Jet.mass, Jet.btag, Jet.id, btag_max1_number_tr, btag_max2_number_tr, pt_max1_number_tr, pt_max2_number_tr, 1., Bjet1_tr, Bjet2_tr, Qjet1_tr, Qjet2_tr, qq_tr); 
		Float_t Mqq_tr = qq_tr.M();
		Float_t bbDeltaPhi_tr = TMath::Abs(Bjet1_tr.DeltaPhi(Bjet2_tr));
		Float_t qqDeltaEta_tr = TMath::Abs(Qjet1_tr.Eta()-Qjet2_tr.Eta());
/////////////////////////////
			Float_t trigger_weight=0.;
		//	if (	SingleBtagVBFTriggerWeight(Jet.pt[0],Jet.pt[1],Jet.pt[2],Jet.pt[3],CSV1, Detaqq_eta , Mqq_eta ,  bbDeltaPhi_tr, qqDeltaEta_tr)>1.e-05)	
		//		trigger_weight=SingleBtagVBFTriggerWeight(Jet.pt[0],Jet.pt[1],Jet.pt[2],Jet.pt[3],CSV1, Detaqq_eta , Mqq_eta ,  bbDeltaPhi_tr, qqDeltaEta_tr);
	//		genweight*=trigger_weight;
	}



		if ((BG[files]==1)&&(set_type==0)) {
			Float_t trigger_weight=0.;
	//		if (DoubleBtagVBFTriggerWeight(Jet.pt[0], Jet.pt[1], Jet.pt[2], Jet.pt[3],CSV1, CSV2, Detaqq_eta,Mqq_eta, qqDeltaEta, Mqq)>1.e-05)
			//	trigger_weight = DoubleBtagVBFTriggerWeight(Jet.pt[0], Jet.pt[1], Jet.pt[2], Jet.pt[3],CSV1, CSV2, Detaqq_eta,Mqq_eta, qqDeltaEta, Mqq) ;
		//	genweight*=trigger_weight;
		}

////////////////////////////



/*
		Float_t Mqq_parametersQCD[3] = {};
		Float_t Mqq_parametersData[3] = {};
		if (set_type==1)  {Mqq_parametersQCD[0]= 1. ; Mqq_parametersQCD[1]=5.42e02 ;Mqq_parametersQCD[2]=3.99e02;}
		if (set_type==0)  {Mqq_parametersQCD[0]= 1. ; Mqq_parametersQCD[1]=1.77e02 ;Mqq_parametersQCD[2]=8.2e1;}
		if (set_type==1)	{ Mqq_parametersData[0]= 1. ;  Mqq_parametersData[1]= 6.31e02 ; Mqq_parametersData[2]= 1.64e02;}
		if (set_type==0) { Mqq_parametersData[0]= 1. ; Mqq_parametersData[1]=  1.82e02 ; Mqq_parametersData[2]= 4.41e1;}
		TF1 *funcQCD = new TF1("erffuncQCD",erf,0.,5000.,3);
		funcQCD->FixParameter(0,Mqq_parametersQCD[0]);
		funcQCD->FixParameter(1,Mqq_parametersQCD[1]);
		funcQCD->FixParameter(2,Mqq_parametersQCD[2]);

		TF1 *funcData = new TF1("erffuncData",erf,0.,5000.,3);
		funcData->FixParameter(0,Mqq_parametersData[0]);
		funcData->FixParameter(1,Mqq_parametersData[1]);
		funcData->FixParameter(2,Mqq_parametersData[2]);


	//	if (data[files]!=1)	genweight*=funcData->Eval(Mqq)/funcQCD->Eval(Mqq);

*/


		presel+=genweight0;

		presel_vtype[(int)(v_type+1)]+=genweight0;


		TLorentzVector bb;
		bb = Bjet1+Bjet2;
		Float_t Mbb = bb.M();

		Float_t alpha_bjet1_reg = Jet.pt_regVBF[btag_max1_number]/Jet.pt[btag_max1_number];
		Float_t alpha_bjet2_reg = Jet.pt_regVBF[btag_max2_number]/Jet.pt[btag_max2_number];
		Float_t Mbb_reg = (alpha_bjet1_reg*Bjet1 + alpha_bjet2_reg*Bjet2).M(); 
		Float_t Mbb_reg_fsr;
		int fsr_bjet=0;

		TLorentzVector FSRjet;
		for (int i=0;i<nJets;i++){
			if ((i!=btag_max1_number)&&(i!=btag_max2_number)&&(i!=pt_max1_number)&&(i!=pt_max2_number)){
				FSRjet.SetPtEtaPhiM(Jet.pt[i],Jet.eta[i],Jet.phi[i],Jet.mass[i]);
				if ((FSRjet.DeltaR(Bjet1)<0.8) || (FSRjet.DeltaR(Bjet2)<0.8)) {
					fsr_bjet++;
					break;
				} 
			}
		}

		if (fsr_bjet>0) Mbb_reg_fsr=(alpha_bjet1_reg*Bjet1 + alpha_bjet2_reg*Bjet2 + FSRjet).M();
		else  Mbb_reg_fsr=(alpha_bjet1_reg*Bjet1 + alpha_bjet2_reg*Bjet2).M();


		
	


		TLorentzVector bbqq;
		bbqq = Bjet1 + Bjet2 + Qjet1 + Qjet2;

		Float_t cosObbqq =TMath::Cos( ( ( Bjet1.Vect() ).Cross(Bjet2.Vect()) ).Angle( ( Qjet1.Vect() ).Cross(Qjet2.Vect()) ) );	

		Float_t EtaBQ1;
	 	Float_t EtaBQ2;
		Float_t PhiBQ1; 	
		Float_t PhiBQ2;
		 if (Qjet1.Eta() >= Qjet2.Eta()) {
			if (Bjet1.Eta() >= Bjet2.Eta())  {
				EtaBQ1 = Qjet1.Eta()-Bjet1.Eta();
				PhiBQ1 = TMath::Abs(Bjet1.DeltaPhi(Qjet1));		
			}
			else {
				EtaBQ1 = Qjet1.Eta()-Bjet2.Eta();
				PhiBQ1 = TMath::Abs(Bjet2.DeltaPhi(Qjet1));	
			}	
		} else if (Bjet1.Eta() >= Bjet2.Eta()) {
				EtaBQ1 = Qjet2.Eta()-Bjet1.Eta();
				PhiBQ1 = TMath::Abs(Bjet1.DeltaPhi(Qjet2));	
				
				}
			else {
				EtaBQ1 = Qjet2.Eta()-Bjet2.Eta();
				PhiBQ1 = TMath::Abs(Bjet2.DeltaPhi(Qjet2));	
			}


		 if (Qjet1.Eta() <= Qjet2.Eta()) {
			if (Bjet1.Eta() <= Bjet2.Eta())  {
				EtaBQ2 = Qjet1.Eta()-Bjet1.Eta();
				PhiBQ2 = TMath::Abs(Bjet1.DeltaPhi(Qjet1));		
			}
			else {
				EtaBQ2 = Qjet1.Eta()-Bjet2.Eta();
				PhiBQ2 = TMath::Abs(Bjet2.DeltaPhi(Qjet1));	
			}	
		} else if (Bjet1.Eta() <= Bjet2.Eta()) {
				EtaBQ2 = Qjet2.Eta()-Bjet1.Eta();
				PhiBQ2 = TMath::Abs(Bjet1.DeltaPhi(Qjet2));	
				
				}
			else {
				EtaBQ2 = Qjet2.Eta()-Bjet2.Eta();
				PhiBQ2 = TMath::Abs(Bjet2.DeltaPhi(Qjet2));	
			}
		


		Float_t Etot = Bjet1.E()+Bjet2.E()+Qjet1.E()+Qjet2.E();
		Float_t PzTot = Bjet1.Pz()+Bjet2.Pz()+Qjet1.Pz()+Qjet2.Pz();
		Float_t PxTot = Bjet1.Px()+Bjet2.Px()+Qjet1.Px()+Qjet2.Px();
		Float_t PyTot = Bjet1.Py()+Bjet2.Py()+Qjet1.Py()+Qjet2.Py();
	
		Float_t x1 = 0.;
		Float_t x2 = 0.;
		x1 = (Etot + PzTot)/2./13000.;
		x2 = (Etot - PzTot)/2./13000.;

		TLorentzVector q1,q2,q1_after,q2_after, VB1, VB2;
		q1.SetPxPyPzE(0.,0.,13000./2.*x1,13000./2.*x1);
		q2.SetPxPyPzE(0.,0.,-13000./2.*x2,13000./2.*x2);
		q1_after.SetPxPyPzE(Qjet1.Px()/Qjet1.Beta(),Qjet1.Py()/Qjet1.Beta(),Qjet1.Pz()/Qjet1.Beta(),Qjet1.E());
		q2_after.SetPxPyPzE(Qjet2.Px()/Qjet2.Beta(),Qjet2.Py()/Qjet2.Beta(),Qjet2.Pz()/Qjet2.Beta(),Qjet2.E());
		if (q1_after.Eta()>=0.) {
			VB1 = -q1_after+q1;
			VB2 = -q2_after+q2;
		} else {
			VB1 = -q2_after+q1;
			VB2 = -q1_after+q2;
		} 
		Float_t VB1_mass, VB2_mass;
		VB1_mass = TMath::Abs(VB1.M());
		VB2_mass = TMath::Abs(VB2.M());


            hJet1_pt->Fill(Jet.pt[0],genweight);
            hJet1_eta->Fill(Jet.eta[0],genweight);
            hJet1_phi->Fill(Jet.phi[0],genweight);
            hJet2_pt->Fill(Jet.pt[1],genweight);
            hJet2_eta->Fill(Jet.eta[1],genweight);
            hJet2_phi->Fill(Jet.phi[1],genweight);
            hJet3_pt->Fill(Jet.pt[2],genweight);
            hJet3_eta->Fill(Jet.eta[2],genweight);
            hJet3_phi->Fill(Jet.phi[2],genweight);
            hJet4_pt->Fill(Jet.pt[3],genweight);
            hJet4_eta->Fill(Jet.eta[3],genweight);
            hJet4_phi->Fill(Jet.phi[3],genweight);
            hJet5_pt->Fill(Jet.pt[4],genweight);
            hVtype->Fill(v_type,genweight);
			hMqq->Fill(Mqq,genweight);
			hEtaQQ->Fill(qqDeltaEta,genweight);
			hPhiQQ->Fill(qqDeltaPhi,genweight);
			hPhiBB->Fill(bbDeltaPhi,genweight);
			hEtaSoftJets->Fill(Jet.soft_eta[0],genweight);
			hPtSoftJets->Fill(Jet.soft_pt[0],genweight);
			hPtSoftJets2->Fill(Jet.soft_pt[1],genweight);
			hPtSoftJets3->Fill(Jet.soft_pt[2],genweight);
			hMassSoftJets->Fill(Jet.soft_mass[0],genweight);
			hHTsoft->Fill(Jet.HTsoft,genweight);
			hSoft_n2->Fill(Jet.nsoft2, genweight);
			hSoft_n5->Fill(Jet.nsoft5, genweight);
			hSoft_n10->Fill(Jet.nsoft10, genweight);
			hMbb->Fill(Mbb,genweight);
			hqgl->Fill(Jet.qgl[pt_max1_number],genweight);
			hbtag->Fill(Jet.btag[btag_max1_number],genweight);
			hqgl2->Fill(Jet.qgl[pt_max2_number],genweight);
			hbtag2->Fill(Jet.btag[btag_max2_number],genweight);
			hcosOqqbb->Fill(cosObbqq,genweight);
			hEtaQB1->Fill(EtaBQ1,genweight);
			hEtaQB2->Fill(EtaBQ2,genweight);
			hPhiQB1->Fill(PhiBQ1,genweight);
			hPhiQB2->Fill(PhiBQ2,genweight);
			hx1->Fill(x1,genweight);
			hx2->Fill(x2,genweight);
			hVB1_mass->Fill(VB1_mass,genweight);
			hVB2_mass->Fill(VB2_mass,genweight);
			hEtot->Fill(Etot,genweight);
			hPxtot->Fill(PxTot,genweight);
			hPytot->Fill(PyTot,genweight);
			hPztot->Fill(PzTot,genweight);
			hPtqqbb->Fill(bbqq.Pt(),genweight);
			hPhiqqbb->Fill((-1)*bbqq.Phi(),genweight);
			hEtaqqbb->Fill(bbqq.Eta(), genweight);
            hJet1_pt_bin->Fill(Jet.pt[0],genweight);
            hJet2_pt_bin->Fill(Jet.pt[1],genweight);
            hJet3_pt_bin->Fill(Jet.pt[2],genweight);
            hJet4_pt_bin->Fill(Jet.pt[3],genweight);
				hMqq_bin->Fill(Mqq,genweight);
				hnPVs->Fill(nPVs,genweight);
				hMbb_regVBF->Fill(Mbb_reg,genweight);
				hMbb_regVBF_fsr->Fill(Mbb_reg_fsr,genweight);
				hJet1q_pt->Fill(Jet.pt[pt_max1_number],genweight);
				hJet1q_eta->Fill(Jet.eta[pt_max1_number],genweight);
				hJet1q_ptd->Fill(Jet.ptd[pt_max1_number],genweight);
				hJet1q_axis2->Fill(Jet.axis2[pt_max1_number],genweight);
				hJet1q_mult->Fill(Jet.mult[pt_max1_number],genweight);
				hJet2q_pt->Fill(Jet.pt[pt_max2_number],genweight);
				hJet2q_eta->Fill(Jet.eta[pt_max2_number],genweight);
				hJet2q_ptd->Fill(Jet.ptd[pt_max2_number],genweight);
				hJet2q_axis2->Fill(Jet.axis2[pt_max2_number],genweight);
				hJet2q_mult->Fill(Jet.mult[pt_max2_number],genweight);
				hblike1->Fill(Jet.blike_VBF[btag_max1_number],genweight);
				hblike2->Fill(Jet.blike_VBF[btag_max2_number],genweight);
				hmet->Fill(met_pt,genweight);
				hselLeptons_tightId->Fill(selLeptons_tightId[0],genweight);
			
				Float_t logIso  = -5;
				if (selLeptons_relIso03[0] > TMath::Exp(-5)) logIso = TMath::Log(selLeptons_relIso03[0]);
				hselLeptons_relIso03->Fill(logIso,genweight);
				logIso  = -5;
				if (selLeptons_chargedHadRelIso03[0] > TMath::Exp(-5)) logIso = TMath::Log(selLeptons_chargedHadRelIso03[0]);
				hselLeptons_chargedHadRelIso03->Fill(logIso,genweight);
				logIso  = -5;
				if (selLeptons_pfRelIso03[0] > TMath::Exp(-5)) logIso = TMath::Log(selLeptons_pfRelIso03[0]);
				hselLeptons_pfRelIso03->Fill(logIso,genweight);
 
			hqgl1_VBF->Fill(Jet.qgl1_VBF[pt_max1_number],genweight);
			hqgl2_VBF->Fill(Jet.qgl2_VBF[pt_max2_number],genweight);

		//		cout<<"first quark jet candidate, entry =  "<< entry<< "  , ptd = "<< Jet.ptd[pt_max1_number]<< " , axis2 = "<<Jet.axis2[pt_max1_number]<<" , mult = " << Jet.mult[pt_max1_number]<< " , qgl = "<<Jet.qgl[pt_max1_number]<<endl;
	//			cout<<"second quark jet candidate, entry =  "<< entry<< "  , ptd = "<< Jet.ptd[pt_max2_number]<< " , axis2 = "<<Jet.axis2[pt_max2_number]<<" , mult = " << Jet.mult[pt_max2_number]<< " , qgl = "<<Jet.qgl[pt_max2_number]<<endl;


		
			if (genweight>0) pos_weight_presel++;
				
			global_counter++;
        }
		TFile file("output_hist/v14/skimmed_tree"+region[region_type]+dataset_type[set_type]+type[files]+"_v14_qgl2.root","recreate");
    
		for (int i=0;i<numArray;++i){
    	    	histArray[i]->SetLineWidth(2);
    	   	histArray[i]->GetYaxis()->SetTitle("N_{events}");
       		histArray[i]->GetYaxis()->SetTitleFont(42);
       		histArray[i]->GetYaxis()->SetTitleSize(0.060);
        		histArray[i]->GetYaxis()->SetTitleOffset(0.8);
        		histArray[i]->SetLineColor(kBlue);
        		histArray[i]->Draw();
        		histArray[i]->Write();
   		} 
    		file.Write();
    		file.Close();
	 ofstream out("output_txt/v14/skimmed"+region[region_type]+dataset_type[set_type]+type[files]+"_v14_qgl2.txt");
	out<< "preselection only = "<< presel<<" , all evetns in the begining = "<<events_generated<<", % = "<< (float)presel/events_generated<< "N evets 1 fb-1 = "<<(float)presel/events_generated*xsec[files]*1000.<<endl;
	out<<"positive weight in so many events : "<<  pos_weight_presel<<endl;
	out<<"rates for v_type : " <<endl;
	for (int i=0;i<7;i++){
		out<<i-1<<"    "<<(float)presel_vtype[i]/events_generated*xsec[files]<<endl;
	}
	out.close();
//	cout<<"qq matched ratio : "<<qq_matching/qq_matching_all <<endl;
	files++;
} while (files<files_max); 


return 0;
    
}
