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


#if [ $1 -eq 0] 
#then
#	set_type=_double
#fi
#if [ $1 -eq 1] 
#then
#	set_type=_single
#fi

max=11
#variables_names=(Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 cosOqqbb DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF Etot Jet5_pt x1 x2 VB1 VB2 all) #1
variables_names=(Mqq DeltaPhiQQ HTsoft DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF Jet5_pt x2 VB1  all) #1
#variables_names=(Mqq DeltaPhiQQ SoftN5  HTsoft Jet5_pt CSV1 CSV2 DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #1
#variables_names=(Mqq DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #1
#variables_names=(DeltaPhiQQ SoftN5 CSV1 CSV2 DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #1
#variables_names=(SoftN5 CSV1 CSV2 DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #1
#variables_names=(Mqq DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2  DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF Jet5_pt VB2 all) #2
#variables_names=(Mqq DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2  DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF VB2 all) #3
#variables_names=(Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 cosOqqbb  DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #4
#variables_names=(Mqq DeltaPhiQQ SoftN5  HTsoft Jet5_pt CSV1 CSV2 DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #final
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
	qsub -q short.q batch2.sh ${variables_names[counter]} $set_type
	counter=$(( $counter + 1 ))
done
