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

def getSystList() :
    susyTools = ROOT.ST.SUSYObjDef_xAOD("getlist")
    susyTools.setProperty(int)("DataSource", 0)
    susyTools.setProperty("ConfigFile", "SUSYTools/SUSYTools_Default.conf")

    logging.info("initializing SUSYTools")
    susyTools.initialize()

    registry = ROOT.CP.SystematicRegistry.getInstance()
    recommendedSystematics = registry.recommendedSystematics()
    return recommendedSystematics

# create a new sample handler to describe the data files we use
logging.info("creating new sample handler")
sh_all = ROOT.SH.SampleHandler()

commonOptions.fillSampleHandler(sh_all, options.inputDS, options.gridUser, options.gridTag)

sh_all.setMetaString ("nc_tree", "CollectionTree");
#sh_all.printContent();

# this is the basic description of our job
logging.info("creating new job")
job = ROOT.EL.Job()
job.sampleHandler(sh_all)
job.useXAOD()

logging.info("creating algorithms")

#susyTools = setupST()

#here we add the algorithms we want to run over
import collections
algsToRun = collections.OrderedDict()

outputFilename = "trees"
output = ROOT.EL.OutputStream(outputFilename);

algsToRun["basicEventSelection"]       = ROOT.BasicEventSelection()
algsToRun["basicEventSelection"].m_useMetaData  = False
algsToRun["mcEventVeto"]               = ROOT.MCEventVeto()

#todo probably make this a function or something
for syst in getSystList() :
    algsToRun["calibrateST_" + syst.name() ]               = ROOT.CalibrateST()
    algsToRun["calibrateST_" + syst.name() ].systName      = syst.name()

    algsToRun["preselectDileptonicWW_" + syst.name() ]     = ROOT.PreselectDileptonicWWEvents()
    algsToRun["selectDileptonicWW_"    + syst.name() ]     = ROOT.SelectDileptonicWWEvents()
    algsToRun["postselectDileptonicWW_" + syst.name() ]    = ROOT.PostselectDileptonicWWEvents()

#todo move the enums to a separate file since they are shared by multiple algs
    algsToRun["calculateRJigsawVariables_" + syst.name() ]                = ROOT.CalculateRJigsawVariables()
    algsToRun["calculateRJigsawVariables_" + syst.name() ].calculatorName = ROOT.CalculateRJigsawVariables.lvlvCalculator
    algsToRun["calculateRegionVars_" + syst.name() ]                      = ROOT.CalculateRegionVars()
    algsToRun["calculateRegionVars_" + syst.name() ].calculatorName       = ROOT.CalculateRegionVars.lvlvCalculator

    for regionName in ["SR","CR1L","CR0L"]:
        tmpWriteOutputNtuple                     = ROOT.WriteOutputNtuple()
        tmpWriteOutputNtuple.outputName          = outputFilename
        tmpWriteOutputNtuple.regionName          = regionName
        tmpWriteOutputNtuple.systName            = syst.name()
        algsToRun["writeOutputNtuple_"+regionName + syst.name()] = tmpWriteOutputNtuple

job.outputAdd(output);

commonOptions.addAlgsFromDict(job , algsToRun , options.verbosity)

if options.nevents > 0 :
    logging.info("Running " + str(options.nevents) + " events")
    job.options().setDouble (ROOT.EL.Job.optMaxEvents, float(options.nevents));

commonOptions.overwriteSubmitDir(options.submitDir , options.doOverwrite)
commonOptions.submitJob         ( job , options.driver , options.submitDir , options.gridUser , options.gridTag)
