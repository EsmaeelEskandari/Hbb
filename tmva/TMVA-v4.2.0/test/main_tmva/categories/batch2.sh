export MYWORKDIR=/afs/cern.ch/work/n/nchernya/tmva/TMVA-v4.2.0/test/main_tmva/categories/
cd $MYWORKDIR
g++ cat.C -g -o cat `root-config --cflags --glibs`
./cat 0 0
