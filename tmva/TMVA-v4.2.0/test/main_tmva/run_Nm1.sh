export WORKDIR=`pwd`
cd $WORKDIR
#g++ tmva.C -g -o run_Nm1_tmva  -lTMVA `root-config --cflags --glibs`
g++ tmva.C -g -o run_Nm1_tmva  `root-config --cflags --glibs`
cp run_Nm1_tmva /shome/nchernya/Hbb/tmva/main_mva/ 
cp TMVAClassification_main.C /shome/nchernya/Hbb/tmva/main_mva/ 
cp tmva.C /shome/nchernya/Hbb/tmva/main_mva/ 
cp TMVAGui.C  /shome/nchernya/Hbb/tmva/main_mva/
cp tmvaglob.C  /shome/nchernya/Hbb/tmva/main_mva/
cp  efficiencies.C /shome/nchernya/Hbb/tmva/main_mva/
#cp /afs/cern.ch/work/n/nchernya/Hbb/main_tmva/main_tmva_tree_VBFHToBB_M-125_13TeV_powheg-ext_v21*.root /shome/nchernya/Hbb/tmva/main_mva/
#cp /afs/cern.ch/work/n/nchernya/Hbb/main_tmva/main_tmva_tree_BTagCSV_v21*.root /shome/nchernya/Hbb/tmva/main_mva/ 

#if [ $1 -eq 0] 
#then
#	set_type=_double
#fi
#if [ $1 -eq 1] 
#then
#	set_type=_single
#fi

max=1  #14
#max=14  #14
#variables_names=(Mbb Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 CSV1 CSV2 cosOqqbb DeltaEtaQB1 DeltaEtaQB DeltaEtaQB2 Jet5_pt x1 x2 axis2_jet1 axis2_jet2 all) #1
#variables_names=(Mbb Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 CSV1 CSV2 cosOqqbb  DeltaEtaQB  Jet5_pt x1 x2 axis2_jet1 axis2_jet2 all) #1
#variables_names=( Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 CSV1 CSV2 Jet5_pt DeltaEtaQB axis2_jet1 axis2_jet2 qqbb_pt qqbb_pz  all ) #1
variables_names=( all ) #1
counter=0
set_type=_double
while  [ $counter -lt $max ]
do
#	./run_Nm1_tmva ${variables_names[counter]} $set_type
	qsub -q short.q batch2.sh ${variables_names[counter]} $set_type
	counter=$(( $counter + 1 ))	
done
counter=0
set_type=_single
while  [ $counter -lt $max ]
do
#	./run_Nm1_tmva ${variables_names[counter]} $set_type	
#	qsub -q short.q batch2.sh ${variables_names[counter]} $set_type
	counter=$(( $counter + 1 ))
done
