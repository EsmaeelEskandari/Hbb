export WORKDIR=`pwd`
cd $WORKDIR

g++ plotter.C -g -o plot `root-config --cflags --glibs`  -lMLP -lXMLIO -lTMVA  

declare -A file_names

path=root://t3se01.psi.ch//store/user/nchernya/Hbb/plotterOutput/v21/
file_name=(
#BTagCSV  
VBFHToBB_M-125 
GluGluHToBB_M-125
ttHTobb_M125
ttHToNonbb_M125
WplusH_HToBB
WminusH_HToBB
ZH_HToBB
QCD_HT100to200
QCD_HT200to300
QCD_HT300to500 
QCD_HT500to700
QCD_HT700to1000
QCD_HT1000to1500
QCD_HT1500to2000
QCD_HT2000toInf
ST_tW 
ST_t-channel_top_4f_inclusiveDecays
ST_t-channel_antitop_4f_inclusiveDecays
ST_s-channel 
DYJetsToLL
WJetsToLNu
WJetsToQQ
DYJetsToQQ
#TT
#TT_madgraph
TT_powheg
WW
ZZ
WZ
#bbHSusy120
bbHToBB_yb2
bbHToBB_ybyt
#VBFHToBB_M-125_amc
#GluGluHToBB_M-125_amc
#VBFHToBB_M-125 
#GluGluHToBB_M-125
)

prefix=''
#postfix=v21_qcd_scale_100
postfix=pdf_pdf_unc
postfix=v21_123jets1_2csv_bdt_trigWt
v=v21
ROOT=.root
dataset_type=(double single)
region=(analysis controlTop controlDY)
#region=(controlTop controlDY)
#region=(controlDY)
applyTrigWeight=1
trigWeightNom=(nom up down)
#trigWeightNom=(up down)
#trigWeightNom=(down)
output_dir=$TMPDIR

#xrdcp $path/BTagCSV_${region[$current_region]}_${dataset_type[1]}_trignone_${v}_$postfix.root /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/
#xrdcp $path/BTagCSV_${region[$current_region]}_${dataset_type[0]}_trignone_${v}_$postfix.root /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/

#xrdcp $path/BTagCSV_${region[$current_region]}_${dataset_type[1]}_trignone_${v}_$postfix.txt /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/
#xrdcp $path/BTagCSV_${region[$current_region]}_${dataset_type[0]}_trignone_${v}_$postfix.txt /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/
#tail -n+21 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/BTagCSV_${region[$current_region]}_${dataset_type[1]}_trignone_${v}_$postfix.txt  | head -n $((1))
#tail -n+21 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/BTagCSV_${region[$current_region]}_${dataset_type[0]}_trignone_${v}_$postfix.txt  | head -n $((1))
current_region=0
current_file=0
current_trigWeight=0
max_files=29 #35
while [ $current_file -lt $max_files  ]
do	 
	#echo ${file_name[$current_region]}
#	xrdcp $path${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trignone_${v}_$postfix.txt /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt/
	#xrdcp $path${file_name[$current_file]}_${region[$current_region]}_${dataset_type[1]}_trignone_${v}_$postfix.txt /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt/
#	tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt/${key}_${region[$current_region]}_${dataset_type[1]}_trignone_${v}_$postfix.txt  | head -n $((1))
#	 tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt/${key}_${region[$current_region]}_${dataset_type[0]}_trignone_${v}_$postfix.txt  | head -n $((1))


#	echo $path${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trig${trigWeightNom[$current_trigWeight]}_${v}_$postfix.txt
#	xrdcp $path${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trig${trigWeightNom[$current_trigWeight]}_${v}_$postfix.txt /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/
#	xrdcp $path${file_name[$current_file]}_${region[$current_region]}_${dataset_type[1]}_trig${trigWeightNom[$current_trigWeight]}_${v}_$postfix.txt /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/
	#tail -n+18 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[1]}_trig${trigWeightNom[$current_trigWeight]}_${v}_$postfix.txt | head -n $((1))
	tail -n+18 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trig${trigWeightNom[$current_trigWeight]}_${v}_$postfix.txt | head -n $((1))
#	tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/qcd_scale/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[1]}_trig${trigWeightNom[$current_trigWeight]}_${v}_${postfix}0.txt | head -n $((1))
	#tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/qcd_scale/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[1]}_trig${trigWeightNom[$current_trigWeight]}_${v}_${postfix}1.txt | head -n $((1))
#	tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/qcd_scale/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trig${trigWeightNom[$current_trigWeight]}_${v}_${postfix}0.txt | head -n $((1))
#	tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/qcd_scale/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trig${trigWeightNom[$current_trigWeight]}_${v}_${postfix}1.txt | head -n $((1))



#	tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trigup_${v}_$postfix.txt | head -n $((1))
	#tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trigdown_${v}_$postfix.txt | head -n $((1))
	echo
	echo
	echo
	echo
#	tail -n+13 /mnt/t3nfs01/data01/shome/nchernya/Hbb/plotter/output_txt_80_200/${file_name[$current_file]}_${region[$current_region]}_${dataset_type[0]}_trig${trigWeightNom[$current_trigWeight]}_${v}_$postfix.txt | head -n $((1))




	current_file=$((current_file + 1))
done
