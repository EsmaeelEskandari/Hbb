typedef std::map<double, int> JetList;
int preselection_single_blike(Int_t nJets, Float_t Jet_pt[300], Float_t Jet_eta[300], Float_t Jet_phi[300], Float_t Jet_mass[300], Float_t Jet_blike_VBF[300], Int_t Jet_id[300],Int_t Jet_puId[300], Int_t& btag_max1_number, Int_t& btag_max2_number, Int_t& pt_max1_number, Int_t& pt_max2_number, Float_t trigger, TLorentzVector& Bjet1,TLorentzVector& Bjet2, TLorentzVector& Qjet1, TLorentzVector& Qjet2,TLorentzVector& qq, Float_t scale=1.){

	JetList jetList_bl, jetList_pt;

	btag_max1_number = -1;
	btag_max2_number = -1;
	pt_max1_number = -1;
	pt_max2_number = -1;

	int not_pass=0;
	
	if (nJets<4) return -1;

	float Jet_pt_first[17];
	int tmp_count = 0;
	for (int i=0;i<nJets;i++){
		if (!((Jet_id[i]>2)&&(Jet_puId[i]>0))) continue;
		Jet_pt_first[tmp_count] = Jet_pt[i]; 
		tmp_count++; 
	}
	if (tmp_count<4) return -1;

	if (!(Jet_pt_first[0]>92.*scale)) not_pass= -2;
	if (!(Jet_pt_first[1]>76.*scale)) not_pass = -3;
	if (!(Jet_pt_first[2]>64.*scale)) not_pass=-4;
	if (!(Jet_pt_first[3]>30.*scale))  not_pass = -5;


		int loopJet_max;
		if (nJets<7) loopJet_max = nJets; 
		else loopJet_max = 7; 

      for(int i=0; i<loopJet_max; i++){
			if (!((Jet_id[i]>2)&&(Jet_puId[i]>0))) continue;
  			jetList_bl[Jet_blike_VBF[i]]=i;
		}
	if (jetList_bl.size()<2) return -1;
	
	JetList::reverse_iterator iJet=jetList_bl.rbegin();
	int cont=0;
	while (cont<2){
 		int i = iJet->second;
		if(cont==0) { Bjet1.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]); btag_max1_number=i; }
		if(cont==1) { Bjet2.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]); btag_max2_number=i;}
		cont++;
		iJet++;
	}
	for(int i=0; i<loopJet_max; i++){
  		if ((i!=btag_max1_number)&&(i!=btag_max2_number)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)) jetList_pt[Jet_pt[i]]=i;
	}
	if (jetList_pt.size()<2) return -1;
	cont=0;
	JetList::reverse_iterator iJet2=jetList_pt.rbegin();
	while (cont<2){
 		int i = iJet2->second;
		if(cont==0) { Qjet1.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]); pt_max1_number=i; }
		if(cont==1) { Qjet2.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]); pt_max2_number=i;}
		cont++;
		iJet2++;
	}

		qq = Qjet1+Qjet2;
		Double_t Mqq = qq.M();
		Double_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Double_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		if (!(Mqq>460*scale)) not_pass=-6;
		if (!(qqDeltaEta>4.1*scale)) not_pass=-7;
		if (!(bbDeltaPhi<1.6*(2-scale))) not_pass =  -8;
		
		if (trigger!=1) not_pass = -9;
	
	return not_pass;
}
