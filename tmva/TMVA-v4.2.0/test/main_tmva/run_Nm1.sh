export WORKDIR=`pwd`
cd $WORKDIR
#g++ tmva.C -g -o run_Nm1_tmva  -lTMVA `root-config --cflags --glibs`
g++ tmva.C -g -o run_Nm1_tmva  `root-config --cflags --glibs`

#if [ $1 -eq 0] 
#then
#	set_type=_double
#fi
#if [ $1 -eq 1] 
#then
#	set_type=_single
#fi

max=19
set_type=_double
variables_names=(Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 cosOqqbb DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF Etot Jet5_pt x1 x2 VB1 VB2 all) #1
#variables_names=(Mqq DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2  DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF Jet5_pt VB2 all) #2
#variables_names=(Mqq DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2  DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF VB2 all) #3
#variables_names=(Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 cosOqqbb  DeltaEtaQB1 DeltaEtaQB2 qgl1_VBF qgl2_VBF all) #4
counter=0
while  [ $counter -lt $max ]
do
	./run_Nm1_tmva ${variables_names[counter]} $set_type
	counter=$(( $counter + 1 ))	
done
counter=0
set_type=_single
while  [ $counter -lt $max ]
do
	./run_Nm1_tmva ${variables_names[counter]} $set_type	
	counter=$(( $counter + 1 ))
done
