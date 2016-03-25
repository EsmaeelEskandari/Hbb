int preselection_single(Int_t nJets, Float_t Jet_pt[300], Float_t Jet_eta[300], Float_t Jet_phi[300], Float_t Jet_mass[300], Float_t Jet_btagCSV[300], Int_t Jet_id[300], Int_t& btag_max1_number, Int_t& btag_max2_number, Int_t& pt_max1_number, Int_t& pt_max2_number, Float_t trigger, TLorentzVector& Bjet1,TLorentzVector& Bjet2, TLorentzVector& Qjet1, TLorentzVector& Qjet2,TLorentzVector& qq, Float_t scale=1.){

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
	}

	if (!(Jet_pt_first[0]>92.*scale)) not_pass= -2;
	if (!(Jet_pt_first[1]>76.*scale)) not_pass = -3;
	if (!(Jet_pt_first[2]>64.*scale)) not_pass=-4;
	if (!(Jet_pt_first[3]>30.*scale))  not_pass = -5;


		int loopJet_min = 4;


		Double_t btag_max = -100.;
		for (int i=0;i<loopJet_min;i++){
			if ((isnan(Jet_btagCSV[i])==1)||(isinf(Jet_btagCSV[i]==1)) ) Jet_btagCSV[i]=1.; 
			if ((Jet_btagCSV[i]>btag_max)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)){
				btag_max=Jet_btagCSV[i];
				btag_max1_number=i;
			}
		}
		if (!((btag_max1_number>=0))) return -1;

		Bjet1.SetPtEtaPhiM(Jet_pt[btag_max1_number],Jet_eta[btag_max1_number],Jet_phi[btag_max1_number],Jet_mass[btag_max1_number]);



		TLorentzVector js[3];
		int jcount = 0;
		int j_num[3] = {};
		for (int i=0;i<4;i++){
			if ((i!=btag_max1_number)&&(Jet_id[i]>2)&&(Jet_puId[i]>0)) {
				js[jcount].SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
				j_num[jcount] = i;
				jcount++;
			}
		}
		if (!(jcount==3)) not_pass = -1;
	
	
		Double_t deltaEtaJet_[3] = {TMath::Abs(js[0].Eta()-js[1].Eta()),TMath::Abs(js[1].Eta()-js[2].Eta()), TMath::Abs(js[0].Eta()-js[2].Eta())};
		int eta_num[3][2] = {{0,1}, {1,2} ,{0,2}};
		Float_t max_deltaEta = 0.;
		int max_deltaEta_num = -1;
		for (int i=0;i<3;i++){
			if (deltaEtaJet_[i]>=max_deltaEta) {
				max_deltaEta = deltaEtaJet_[i];
				max_deltaEta_num = i;
			}
		}
		
		pt_max1_number = j_num[ eta_num[max_deltaEta_num][0]];
		pt_max2_number = j_num[ eta_num[max_deltaEta_num][1]];

		if (!((pt_max1_number>=0)&&(pt_max2_number>=0))) return -1;
	
		Qjet1.SetPtEtaPhiM(Jet_pt[pt_max1_number] ,Jet_eta[pt_max1_number],Jet_phi[pt_max1_number],Jet_mass[pt_max1_number]);
	
		Qjet2.SetPtEtaPhiM(Jet_pt[pt_max2_number],Jet_eta[pt_max2_number],Jet_phi[pt_max2_number],Jet_mass[pt_max2_number]);

		for (int i=0;i<4;i++){
			if ( (i!=btag_max1_number)&&(i!=pt_max1_number)&&(i!=pt_max2_number)/*&&(Jet_id[i]>0)*/) btag_max2_number=i;
		}

		if (!((btag_max2_number>=0)))  not_pass =  -1;
		Bjet2.SetPtEtaPhiM(Jet_pt[btag_max2_number],Jet_eta[btag_max2_number],Jet_phi[btag_max2_number],Jet_mass[btag_max2_number]);
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
