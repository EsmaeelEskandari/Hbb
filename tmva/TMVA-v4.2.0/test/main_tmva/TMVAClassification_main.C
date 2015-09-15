// @(#)root/tmva $Id$
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

using namespace std;

void TMVAClassification_main( TString myMethodList = "" )
{
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the
   // corresponding lines from .rootrc

   // methods to be processed can be given as an argument; use format:
   //
   // mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
   //
   // if you like to use a method via the plugin mechanism, we recommend using
   //
   // mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
   // (an example is given for using the BDT as plugin (see below),
   // but of course the real application is when you write your own
   // method based)

   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   Use["Fisher"]          = 0;
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   // --- Boosted Decision Trees
   Use["BDTB"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTG"]            = 1; 
   // 
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
 //  TString outfileName( "TMVA_bjet_new_powheg.root" );
   TString outfileName( "output/TMVA_main_1_step_qcd_300to500_single.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is 
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
  // factory->AddVariable( "myvar1 := var1+var2", 'F' );
  // factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );


////////////first group////////////
   factory->AddVariable( "Mqq", "M_{qq}", "GeV", 'F' );
   factory->AddVariable( "DeltaEtaQQ", "#Delta#eta_{qq}", "", 'F' );
/*
////////////second group//////////////
   factory->AddVariable( "SoftN5", "Soft multiplicity with p_{T} > 5 GeV", "", 'I' );
   factory->AddVariable( "HTsoft", "H_{T}^{soft}", "GeV", 'F' );

////////////third group///////////////
   factory->AddVariable( "CSV1", "CSV_{1}", "", 'F' );
   factory->AddVariable( "CSV2", "CSV_{2}", "", 'F' );

////////////forth group//////////////
   factory->AddVariable( "cosOqqbb", "cos#theta_{qqbb}", "", 'F' );


////////////fifth group//////////
   factory->AddVariable( "DeltaPhiBB", "#Delta#phi_{bb}", "", 'F' );


////////////sixth group//////////
   factory->AddVariable( "DeltaEtaQB1", "#Delta#eta_{qb}^{forward}", "", 'F' );
   factory->AddVariable( "DeltaEtaQB2", "#Delta#eta_{qb}^{backward}", "", 'F' );

*/



   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables
  // factory->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
  // factory->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );

   // Read training and test data
   // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
	TString fname_signal ="/afs/cern.ch/work/n/nchernya/Hbb/main_tmva/main_tmva_tree_VBF_powheg_125_single.root";
	TString fname_bg ="/afs/cern.ch/work/n/nchernya/Hbb/main_tmva/main_tmva_tree_QCD_300_500_single.root";



//	TString fname = "/afs/cern.ch/work/n/nchernya/Hbb/b_likelihood/spring15_M125_powheg_VBF_b_likelihood.root";	

   if (gSystem->AccessPathName( fname_signal )) { // file does not exist in local directory
		cout<<"input file "<< fname_signal<<" doesn't exist!"<<endl;
		//break;
	}
   if (gSystem->AccessPathName( fname_bg)) { // file does not exist in local directory
		cout<<"input file "<< fname_bg<<" doesn't exist!"<<endl;
		//break;
	}
    //  gSystem->Exec("wget http://root.cern.ch/files/tmva_class_example.root");
   
   TFile *input_signal = TFile::Open( fname_signal );
   TFile *input_bg = TFile::Open( fname_bg );
   
   std::cout << "--- TMVAClassification       : Using input signal file: " << input_signal->GetName() << std::endl;
   std::cout << "--- TMVAClassification       : Using input bg file: " << input_bg->GetName() << std::endl;
   
   // --- Register the training and test trees

   TTree *signal     = (TTree*)input_signal->Get("TMVA");
   TTree *bg = (TTree*)input_bg->Get("TMVA");
   
   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t bgWeight = 1.0;
   
   // You can add an arbitrary number of signal or background trees
 //  factory->AddSignalTree    ( bjet,     bjetWeight  );
//   factory->AddBackgroundTree( qjet, qjetWeight );
   factory->AddSignalTree    ( signal,     signalWeight  );
   factory->AddBackgroundTree( bg, bgWeight );
   // Apply additional cuts on the signal and background samples (can be different)
 //  TCut mycuts = "Jet_b_matched==1 && Jet_q_matched==0"; 
//   TCut mycutb = "Jet_b_matched==0"; // for example: TCut mycutb = "abs(var1)<0.5";
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";
   


   // Tell the factory how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used 
   // for training, and the other half for testing:
   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
   // To also specify the number of testing events, use:
   //    factory->PrepareTrainingAndTestTree( mycut,
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   if (Use["MLPBNN"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators


   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD",
                           "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

   if (Use["BDTG"]) //
      factory->BookMethod( TMVA::Types::kBDT, "BDTG",
		   "!H:!V:NTrees=120:MinNodeSize=5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3:NegWeightTreatment=IgnoreNegWeightsInTraining" );

   // For an example of the category classifier usage, see: TMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

   // ---- STILL EXPERIMENTAL and only implemented for BDT's ! 
   // factory->OptimizeAllMethods("SigEffAt001","Scan");
   // factory->OptimizeAllMethods("ROCIntegral","FitGA");

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;

   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVAGui( outfileName );
}

