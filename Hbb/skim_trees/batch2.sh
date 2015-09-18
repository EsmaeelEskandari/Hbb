export MYBATCHDIR=/afs/cern.ch/work/n/nchernya/Hbb/skim_trees/
cd $MYBATCHDIR
g++ run_skim_trees_double_main.C -g -o run `root-config --cflags --glibs` && ./run $1 $2 $3

