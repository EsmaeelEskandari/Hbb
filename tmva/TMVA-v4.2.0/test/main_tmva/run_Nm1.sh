export WORKDIR=`pwd`
cd $WORKDIR

max=18
counter=0
#if [ $1 -eq 0] 
#then
#	set_type=_double
#fi
#if [ $1 -eq 1] 
#then
#	set_type=_single
#fi

set_type=_double
variables_names=(Mqq DeltaEtaQQ DeltaPhiQQ SoftN5 HTsoft CSV1 CSV2 cosOqqbb DeltaEtaQB1 DeltaEtaQB2 qgl1 qgl2 Etot Jet5_pt x1 x2 VB1 VB2)
while  [ $counter < $max ]
do
	./run_Nm1_tmva ${variables_names[counter]} $set_type	
done
set_type=_single
while  [ $counter < $max ]
do
	./run_Nm1_tmva ${variables_names[counter]} $set_type	
done
