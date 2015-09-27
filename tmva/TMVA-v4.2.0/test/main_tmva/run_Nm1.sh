export WORKDIR=`pwd`
cd $WORKDIR
g++ tmva.C -g -o run_Nm1_tmva `root-config --cflags --glibs`

#if [ $1 -eq 0] 
#then
#	set_type=_double
#fi
#if [ $1 -eq 1] 
#then
#	set_type=_single
#fi

max=12  #-1   18
set_type=_double
#variables_names=(Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 cosOqqbb DeltaEtaQB1 DeltaEtaQB2 qgl1 qgl2 Etot Jet5_pt x1 x2 VB1 VB2 all)
variables_names=(Mqq DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2  DeltaEtaQB1 DeltaEtaQB2 qgl1 qgl2 Jet5_pt VB2 all)
counter=0
while  [ $counter -le $max ]
do
	./run_Nm1_tmva ${variables_names[counter]} $set_type
	counter=$(( $counter + 1 ))	
done
counter=0
set_type=_single
while  [ $counter -le $max ]
do
	./run_Nm1_tmva ${variables_names[counter]} $set_type	
	counter=$(( $counter + 1 ))
done
