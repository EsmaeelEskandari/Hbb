export WORKDIR=`pwd`
echo "Working on a machine:" `uname -a`
cd $WORKDIR

output_dir=(VBFHToBB_M-125_13TeV_powheg-ext
 VBFHToBB_M-125_13TeV_powheg 
QCD_HT100to200
 QCD_HT200to300
 QCD_HT300to500
 QCD_HT500to700
 QCD_HT700to1000
 QCD_HT1000to1500
 QCD_HT1500to2000 
QCD_HT2000toInf
 TTJets_TuneCUETP8M1_13TeV-madgraph
 TT_TuneCUETP8M1_13TeV-amcatnlo
 DYJetsToQQ WJetsToQQ_HT180_13TeV-madgraph
 DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo_ext_1
 DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo_ext_2
 DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo
 ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg 
ST_tW_top_5f_inclusiveDecays_13TeV-powheg
 ST_s-channel_4f_leptonDecays_13TeV-amcatnlo
 WJetsToLNu_TuneCUETP8M1_13TeV-amcatnlo
 GluGluHToBB_M125_13TeV_powheg-ext
 GluGluHToBB_M125_13TeV_powheg 
ttHTobb_M125_13TeV_powheg
 ttHToNonbb_M125_13TeV_powheg )
max_samples=25
current_sample=0 
while [ $current_sample -lt $max_samples ]
#while [ $current_sample -gt 4 ] && [  $current_sample -lt 8  ]
do
#	rm list${output_dir[current_sample]}_single.txt
#	rm list${output_dir[current_sample]}_single2.txt
#	path=root://t3dcachedb.psi.ch:1094///pnfs/psi.ch/cms/trivcat/
#	path2=/store/user/nchernya/Hbb/v21/workflow/${output_dir[current_sample]}_single/
#	path3=/store/user/nchernya/Hbb/v21/workflow/
#	xrdfs t3se01.psi.ch rm $path2${output_dir[current_sample]}_single_v21.root
#	xrdfs t3se01.psi.ch ls $path2 > list${output_dir[current_sample]}_single.txt
#	
#	awk -v path="$path" '{print path $0}' list${output_dir[current_sample]}_single.txt > list${output_dir[current_sample]}_single2.txt
#	rm list${output_dir[current_sample]}_single.txt
		

	echo xrdcp  /scratch/$USER/${output_dir[current_sample]}_single_v21.root root://t3se01.psi.ch//store/user/nchernya/Hbb/v21/work/
#	echo hadd -f -k -v 99  /scratch/$USER/${output_dir[current_sample]}_single_v21.root  cat list${output_dir[current_sample]}_single2.txt | xargs 
	current_sample=$((current_sample + 1))
done
