export WORKDIR=`pwd`
echo "Working on a machine:" `uname -a`
cd $WORKDIR
g++ run_skim_trees_double_main.C -g -o run `root-config --cflags --glibs`
g++ run_skim_trees_single_main.C -g -o run_single `root-config --cflags --glibs`

EOS=root://xrootd-cms.infn.it//store/user/arizzi/VHBBHeppyV21/
TREE=tree_
ROOT=.root
WORKDIR2=/scratch/nchernya

#output_dir=(/QCD_HT100to200 /QCD_HT200to300 /QCD_HT300to500 /QCD_HT500to700 /QCD_HT700to1000 /QCD_HT1000to1500 /QCD_HT1500to2000 /QCD_HT2000toInf /TTbar1 /TTbar2 /DYtoQQ /DYtoLL /DYtoLLamc /ST_tWtop /ST_tWantitop /WJetsToLNu /VBFHToBB_M-125_13TeV_powheg /BTagCSV_C /BTagCSV_D)
output_dir=(QCD_HT100to200 QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500 QCD_HT1500to2000 QCD_HT2000toInf JetHT_C JetHT_D)
v14=_v20
v20=_v20
max_trees_number=(964 232 247 239 169 65 51 29 32 806) 
current_sample=0
while read sample
do
#	if [ $current_sample -lt 3 ] || [ $current_sample -gt 7 ]
#	if [ $current_sample -lt 17 ] || [ $current_sample -gt 18 ]
#	then
#		current_sample=$((current_sample + 1))
#		continue
#	fi
	
	cd $WORKDIR
	FILES=$EOS$sample$TREE
	current_tree=1
	while [ $current_tree -le ${max_trees_number[current_sample]} ]
#	while [ $current_tree -le 1 ]
	do
		f=$FILES$current_tree$ROOT
		echo $f
	#	qsub -q short.q batch2.sh $f $current_tree ${output_dir[current_sample]}  # old
#		echo $f $current_tree $WORKDIR/v14${output_dir[current_sample]}$v14 #old
		qsub -q short.q batch21_trig.sh $f $current_tree ${output_dir[current_sample]} #old
#		./run_single $f $current_tree $WORKDIR/v14${output_dir[current_sample]}$v14
#		break
#		qsub -q short.q batch2_single.sh $f $current_tree $scratch${output_dir[current_sample]}$v14 
#		if [ $current_sample -eq 12 ]
#			then  
#				qsub -q short.q batch2.sh $f $current_tree $WORKDIR${output_dir[current_sample]}$v14
#		fi
#		break
 		current_tree=$(( $current_tree + 1 ))
	done
#	break
	current_sample=$((current_sample + 1))
#	if [ $current_sample -eq 11 ]
#	then
#		break		
#	fi 
done < samples_paths_v21.txt
