export WORKDIR=`pwd`
cd $WORKDIR

g++ plotter_pdf.C -g -o pdf `root-config --cflags --glibs`  -lMLP -lXMLIO -lTMVA  

declare -A file_names

#path=dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat///store/user/nchernya/Hbb/v21/work/
path=dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/user/nchernya/Hbb/v21/main_mva/
file_names=(
#["BTagCSV"]=BTagCSV
["VBFHToBB_M-125"]=VBFHToBB_M-125_13TeV_powheg-ext 
["GluGluHToBB_M-125"]=GluGluHToBB_M125_13TeV_powheg-ext
#["ttHTobb_M125"]=ttHTobb_M125_13TeV_powheg
#["ttHToNonbb_M125"]=ttHToNonbb_M125_13TeV_powheg
#["WplusH_HToBB"]=WplusH_HToBB
#["WminusH_HToBB"]=WminusH_HToBB
#["ZH_HToBB"]=ZH_HToBB
#["QCD_HT100to200"]=QCD_HT100to200
#["QCD_HT200to300"]=QCD_HT200to300
#["QCD_HT300to500"]=QCD_HT300to500 
#["QCD_HT500to700"]=QCD_HT500to700
#["QCD_HT700to1000"]=QCD_HT700to1000
#["QCD_HT1000to1500"]=QCD_HT1000to1500
#["QCD_HT1500to2000"]=QCD_HT1500to2000
#["QCD_HT2000toInf"]=QCD_HT2000toInf 
#["ST_tW"]=ST_tW_top_5f_inclusiveDecays_13TeV-powheg 
#["ST_s-channel"]=ST_s-channel_4f_leptonDecays_13TeV-amcatnlo 
#["DYJetsToLL"]=DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnlo_ext_1
#["WJetsToLNu"]=WJetsToLNu_TuneCUETP8M1_13TeV-amcatnlo
#["WJetsToQQ"]=WJetsToQQ_HT180_13TeV-madgraph
#["DYJetsToQQ"]=DYJetsToQQ
#["TT"]=TT_TuneCUETP8M1_13TeV-amcatnlo
#["TT_madgraph"]=TTJets_TuneCUETP8M1_13TeV-madgraph
#["TT_powheg"]=TT_powheg
#["WW"]=WW
#["ZZ"]=ZZ
#["WZ"]=WZ
#["bbHSusy120"]=SUSYGluGluToBBHToBB_M-120
#["bbHToBB_yb2"]=bbHToBB_M-125_4FS_yb2
#["bbHToBB_ybyt"]=bbHToBB_M-125_4FS_ybyt
#["VBFHToBB_M-125_amc"]=VBFHToBB_M-125_13TeV_amc
#["GluGluHToBB_M-125_amc"]=GluGluHToBB_M125_13TeV_amc
)
prefix='main_mva_v21_'
#file_end=_single_v21
#file_end=_single
file_end=( _double _single )
#postfix=v21
postfix=pdf
v=v21
ROOT=.root
dataset_type=(double single)
region=(analysis)
applyTrigWeight=1
trigWeightNom=(nom)
output_dir=$TMPDIR


for key in ${!file_names[@]}; do
	current_region=0
	while [ $current_region -lt  1 ] 
	do
		data=0
		current_trigWeight=0
		while [ $current_trigWeight -lt  2 ] 
		do
			f=$path$prefix${file_names[${key}]}${file_end[$current_trigWeight]}.root
			#qsub -q short.q batch_pdf.sh  $f ${key} ${dataset_type[$current_trigWeight]} ${region[$current_region]} $data  $applyTrigWeight ${trigWeightNom[0]} $v $postfix
			qsub -q short.q batch_pdf.sh  $f ${key} ${dataset_type[$current_trigWeight]} ${region[$current_region ]} $data $applyTrigWeight ${trigWeightNom[0]} $v $postfix
			#echo $f ${key} ${dataset_type[$current_trigWeight]} ${region[$current_region ]} $data $applyTrigWeight ${trigWeightNom[0]} $v $postfix
			current_trigWeight=$(( $current_trigWeight + 1 ))
		done
		current_region=$(( $current_region + 1 ))
#		break
	done
#	break
done
