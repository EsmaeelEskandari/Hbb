export WORKDIR=`pwd`
echo "Working on a machine:" `uname -a`
cd $WORKDIR

output_dir=(
#VBFHToBB_M-125_13TeV_powheg-ext
# VBFHToBB_M-125_13TeV_powheg 
#QCD_HT100to200
# QCD_HT200to300
# QCD_HT300to500
# QCD_HT500to700
# QCD_HT700to1000
# QCD_HT1000to1500
# QCD_HT1500to2000 
#QCD_HT2000toInf
# TTJets_TuneCUETP8M1_13TeV-madgraph
# TT_TuneCUETP8M1_13TeV-amcatnlo
# DYJetsToQQ WJetsToQQ_HT180_13TeV-madgraph
# DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo_ext_1
# DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo_ext_2
# DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo
# ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg 
#ST_tW_top_5f_inclusiveDecays_13TeV-powheg
# ST_s-channel_4f_leptonDecays_13TeV-amcatnlo
# WJetsToLNu_TuneCUETP8M1_13TeV-amcatnlo
#GluGluHToBB_M125_13TeV_powheg-ext
#GluGluHToBB_M125_13TeV_powheg 
#ttHTobb_M125_13TeV_powheg
# ttHToNonbb_M125_13TeV_powheg 
#ZZ
#WZ
#WW
#ZH_HToBB
#WplusH_HToBB
#WminusH_HToBB
#TT_powheg_1
#TT_powheg_5
#TT_powheg_2
#TT_powheg_6
#TT_powheg_7
#SUSYGluGluToBBHToBB_M-120
#bbHToBB_M-125_4FS_yb2
#bbHToBB_M-125_4FS_ybyt
#VBFHToBB_M-125_13TeV_amc
#GluGluHToBB_M125_13TeV_amc
ST_t-channel_top_4f_inclusiveDecays
ST_t-channel_antitop_4f_inclusiveDecays 
)
max_samples=2
current_sample=0 
while [ $current_sample -lt $max_samples ]
#while [ $current_sample -gt 4 ] && [  $current_sample -lt 8  ]
do
#	rm list${output_dir[current_sample]}_single.txt
#	rm list${output_dir[current_sample]}_single2.txt
#	path=root://t3dcachedb.psi.ch:1094///pnfs/psi.ch/cms/trivcat/
#	path2=/store/user/nchernya/Hbb/v21/workflow/${output_dir[current_sample]}_single/
#	path3=/store/user/nchernya/Hbb/v21/workflow/
#	xrdfs t3se01.psi.ch ls $path2 > list${output_dir[current_sample]}_single.txt
	
#	awk -v path="$path" '{print path $0}' list${output_dir[current_sample]}_single.txt > list${output_dir[current_sample]}_single2.txt
#	rm list${output_dir[current_sample]}_single.txt
		

	echo hadd -f -k -v 99  /scratch/$USER/${output_dir[current_sample]}_single_v21.root  cat list${output_dir[current_sample]}_single2.txt | xargs 
	current_sample=$((current_sample + 1))
	##############xrdfs t3se01.psi.ch rm $path2${output_dir[current_sample]}_single_v21.root
done
