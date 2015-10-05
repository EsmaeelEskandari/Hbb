int preselection_single(Int_t nJets, Float_t Jet_pt[300], Float_t Jet_eta[300], Float_t Jet_phi[300], Float_t Jet_mass[300], Float_t Jet_btagCSV[300], Int_t Jet_id[300], Int_t& btag_max1_number, Int_t& btag_max2_number, Int_t& pt_max1_number, Int_t& pt_max2_number, Float_t trigger, TLorentzVector& Bjet1,TLorentzVector& Bjet2, TLorentzVector& Qjet1, TLorentzVector& Qjet2,TLorentzVector& qq, Float_t scale=1.){
	
	if (nJets<4) return -1;

	if (!((Jet_pt[0]>92.*scale)&&(Jet_pt[1]>76.*scale)&&(Jet_pt[2]>64.*scale)&&(Jet_pt[3]>30.*scale)))  return -1;

		int loopJet_min = 4;

		Double_t btag_max = 0.7;
		for (int i=0;i<loopJet_min;i++){
			if ((Jet_btagCSV[i]>btag_max)&&(Jet_id[i]>0)){
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
			if ((i!=btag_max1_number)&&(Jet_id[i]>0)) {
				js[jcount].SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
				j_num[jcount] = i;
				jcount++;
			}
		}
		if (!(jcount==3)) return -1;
	
	
		Float_t deltaEtaJet_[3] = {TMath::Abs(js[0].Eta()-js[1].Eta()),TMath::Abs(js[1].Eta()-js[2].Eta()), TMath::Abs(js[0].Eta()-js[2].Eta())};
		int eta_num[3][2] = {{0,1}, {1,2} ,{0,2}};
		Float_t max_deltaEta = 0.;
		int max_deltaEta_num = -1;
		for (int i=0;i<3;i++){
			if (deltaEtaJet_[i]>max_deltaEta) {
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
			if ( (i!=btag_max1_number)&&(i!=pt_max1_number)&&(i!=pt_max2_number)&&(Jet_id[i]>0)) btag_max2_number=i;
		}

		if (!((btag_max2_number>=0)))  return -1;
		Bjet2.SetPtEtaPhiM(Jet_pt[btag_max2_number],Jet_eta[btag_max2_number],Jet_phi[btag_max2_number],Jet_mass[btag_max2_number]);
		qq = Qjet1+Qjet2;
		Double_t Mqq = qq.M();
		Double_t bbDeltaPhi = TMath::Abs(Bjet1.DeltaPhi(Bjet2));
		Double_t qqDeltaEta = TMath::Abs(Qjet1.Eta()-Qjet2.Eta());
		if (!((Mqq>460*scale)&&(qqDeltaEta>4.1*scale)&&(bbDeltaPhi<1.6*(2-scale)))) return -1;
		
		if (trigger!=1) return -1;
	
	return 0;
}
