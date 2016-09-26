import ROOT
# Workaround to fix threadlock issues with GUI
ROOT.PyConfig.StartGuiThread = False
import logging
logging.basicConfig(level=logging.INFO)

import commonOptions

parser = commonOptions.parseCommonOptions()
#you can add additional options here if you want
#parser.add_option('--verbosity', help   = "Run all algs at the selected verbosity.",choices=("info", "warning","error", "debug", "verbose"), default="error")

(options, args) = parser.parse_args()
#print options

ROOT.gROOT.Macro( '$ROOTCOREDIR/scripts/load_packages.C' )

# create a new sample handler to describe the data files we use
logging.info("creating new sample handler")
sh_all = ROOT.SH.SampleHandler()

commonOptions.fillSampleHandler(sh_all, options.inputDS)

sh_all.setMetaString ("nc_tree", "CollectionTree");
#sh_all.printContent();

# this is the basic description of our job
logging.info("creating new job")
job = ROOT.EL.Job()
job.sampleHandler(sh_all)
job.useXAOD()

logging.info("creating algorithms")

outputFilename = "trees"
output = ROOT.EL.OutputStream(outputFilename);

#here we add the algorithms we want to run over
import collections
algsToRun = collections.OrderedDict()

algsToRun["basicEventSelection"]       = ROOT.BasicEventSelection()
commonOptions.configxAODAnaHelperAlg(algsToRun["basicEventSelection"] )
setattr(algsToRun["basicEventSelection"], "m_derivationName"  , "SUSY1KernelSkim" )
setattr(algsToRun["basicEventSelection"], "m_triggerSelection", ".+" )
setattr(algsToRun["basicEventSelection"], "m_applyTriggerCut" , False )


algsToRun["calibrateST"]               = ROOT.CalibrateST()
algsToRun["calibrateST" ].SUSYToolsConfigFileName = "${ROOTCOREBIN}/data/FactoryTools/SUSYTools_zl.conf"
algsToRun["calibrateST" ].systName     = ""
algsToRun["calibrateST" ].PRWConfigFileNames       = algsToRun["basicEventSelection"].m_PRWFileNames
algsToRun["calibrateST" ].PRWLumiCalcFileNames     = algsToRun["basicEventSelection"].m_lumiCalcFileNames

# This preselection algorithm does nothing at the moment.
algsToRun["preselectDileptonicWW"]   = ROOT.PreselectDileptonicWWEvents()

# Main selection and sorting algorithm
algsToRun["selectZeroLepton"]        = ROOT.SelectZeroLeptonEvents()

# These are the calculators that calculate various derived quantities
algsToRun["calculateRJigsawVariables"] = ROOT.CalculateRJigsawVariables()
algsToRun["calculateRJigsawVariables"].calculatorName = ROOT.CalculateRJigsawVariables.zlCalculator
algsToRun["calculateRegionVars"]                      = ROOT.CalculateRegionVars()
algsToRun["calculateRegionVars"].calculatorName       = ROOT.CalculateRegionVars.zlCalculator

# This bit runs filtering on derived variables. e.g. MEff filter.
algsToRun["postselectZeroLepton"]    = ROOT.PostselectZeroLeptonEvents()

# These correspond to writing out the various trees used in the analysis
for regionName in ["SR","CR1L","CR2L","CRY"]:
    tmpWriteOutputNtuple                       = ROOT.WriteOutputNtuple()
    tmpWriteOutputNtuple.outputName            = outputFilename
    tmpWriteOutputNtuple.regionName            = regionName
    tmpWriteOutputNtuple.systName            = ""
    algsToRun["writeOutputNtuple_"+regionName] = tmpWriteOutputNtuple

if options.doSystematics : commonOptions.doSystematics(algsToRun)

job.outputAdd(output);
commonOptions.addAlgsFromDict(job , algsToRun , options.verbosity)

if options.nevents > 0 :
    logging.info("Running " + str(options.nevents) + " events")
    job.options().setDouble (ROOT.EL.Job.optMaxEvents, float(options.nevents));

commonOptions.overwriteSubmitDir(options.submitDir , options.doOverwrite)
commonOptions.submitJob         ( job , options.driver , options.submitDir , options.gridUser , options.gridTag)
