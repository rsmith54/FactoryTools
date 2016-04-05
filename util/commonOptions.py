import os
import logging
import shutil
import ROOT
from copy import copy
from copy import deepcopy

import basicEventSelectionConfig

from optparse import OptionParser

from datetime import date

def parseCommonOptions() :
    parser = OptionParser()
    parser.add_option("--submitDir", help   = "dir to store the output", default="submit_dir")
    parser.add_option("--inputDS", help     = "You can pass either the directory locally, the file containing the list of grid datasets, or directly the name of a grid dataset. ", default="/afs/cern.ch/work/l/larry/public/lvlv_datasets/")

    parser.add_option("--gridUser", help    = "gridUser"  , default= '')
    parser.add_option("--gridTag", help     = "gridTag"   , default= '')

    parser.add_option("--driver", help      = "select where to run", choices=("direct", "prooflite", "lsf","grid"), default="direct")
    parser.add_option('--doOverwrite', help = "Overwrite submit dir if it already exists",action="store_true", default=False)
    parser.add_option('--nevents', help     = "Run n events ", default = -1 )
    parser.add_option('--verbosity', help   = "Run all algs at the selected verbosity.",choices=("info", "warning","error", "debug", "verbose"), default="error")
    parser.add_option('--doSystematics', help = "Do systematics.  This will take *MUCH* longer!",action="store_true", default=False)
    return parser

import atexit
@atexit.register
def quiet_exit():
    ROOT.gSystem.Exit(0)

def setVerbosity ( alg , levelString ) :
    level = None
    if levelString == "info"    : level = ROOT.MSG.INFO
    if levelString == "warning" : level = ROOT.MSG.WARNING
    if levelString == "error"   : level = ROOT.MSG.ERROR
    if levelString == "debug"   : level = ROOT.MSG.DEBUG
    if levelString == "verbose" : level = ROOT.MSG.VERBOSE

    if not level :
        logging.info("you set an illegal verbosity! Exiting.")
        commonOptions.quiet_exit()
    alg.setMsgLevel(level)

def fillSampleHandler ( sh_all, inp ) :
    """You can pass either the directory locally, the file containing the list of grid datasets, or directly the name of a grid dataset. """
    if os.path.isfile(inp) :
        with open(inp) as f :
            for ds in f :
                ROOT.SH.addGrid(sh_all, ds.rstrip() )
    elif os.path.isdir(inp) :
        mylist = ROOT.SH.DiskListLocal(inp)
        ROOT.SH.scanDir(sh_all,mylist, "*")
    else :
        ROOT.SH.scanDQ2(sh_all, inp)

def addAlgsFromDict( job,  algsToRun , verbosity = "error" ) :
    for name,alg in algsToRun.iteritems() :
        setVerbosity(alg , verbosity)
        logging.info("adding " + name + " to algs" )
        alg.SetName(name)#this is needed to see the alg names with athena messaging
        job.algsAdd(alg)

def overwriteSubmitDir (submitDir, doOverwrite) :
    if os.path.isdir(submitDir) :
        logging.info(submitDir + " already exists.")
        if doOverwrite :
            logging.info( "Overwriting previous submitDir")
            shutil.rmtree(submitDir)
        else :
            logging.info( "Exiting.  If you want to overwrite the previous submitDir, use --doOverwrite")
            quiet_exit()

def submitJob (job , driverName , submitDir, gridUser = "" , gridTag = "") :
    if (gridUser or gridTag) and driverName != "grid" :
        logging.error("You have specified a gridUser or gridTag but are not using the grid driver.  Exiting without submitting.")
        quiet_exit()

    logging.info("creating driver")
    driver = None
    if (driverName == "direct"):
        logging.info( "direct driver")
        driver = ROOT.EL.DirectDriver()
        logging.info("submit job")
        driver.submit(job, submitDir)
    elif driverName == "lsf" :
        driver = ROOT.EL.LSFDriver()
        # ROOT.SH.scanNEvents(sh);
        # sh.setMetaDouble(ROOT.EL.Job.optEventsPerWorker, 50000);
        job.options().setString(ROOT.EL.Job.optSubmitFlags, "-q " + "1nh");
        driver.submit(job, submitDir)
    elif (driverName == "prooflite"):
        logging.info( "prooflite")
        driver = ROOT.EL.ProofDriver()
        logging.info("submit job")
        driver.submit(job, submitDir)
    elif (driverName == "grid"):
        logging.info( "grid driver")
        if not gridUser : gridUser = os.environ.get("USER")
        if not gridTag  : gridTag  = date.today().strftime("%m%d%y")
        driver = ROOT.EL.PrunDriver()
        gridOutputDatasetName = "user.%s.%%in:name[2]%%.%%in:name[3]%%.%s"%(gridUser,gridTag)
        driver.options().setString("nc_outputSampleName", gridOutputDatasetName  );
        driver.options().setDouble(ROOT.EL.Job.optGridMergeOutput, 1);

        logging.info("submit job")
        driver.submitOnly(job, submitDir)
    else :
        logging.info( "you gave an illegal driver name.  Not submitting.")


def configBasicEventSelection(alg , configDict = basicEventSelectionConfig.basicEventSelectionDict ) :
    for key, value in configDict.iteritems() :
        if not hasattr(alg, key) :
            raise AttributeError(key)
        setattr(alg, key, value )

def getSystList(dataSource = 1) :
    if( dataSource == 0 ) : return []#for data, just run once with no systematics

    susyTools = ROOT.ST.SUSYObjDef_xAOD("getlist")
    susyTools.setDataSource(dataSource)
    susyTools.setProperty("ConfigFile", "SUSYTools/SUSYTools_Default.conf")

    logging.info("initializing SUSYTools")

    susyTools.initialize()

    registry = ROOT.CP.SystematicRegistry.getInstance()
    recommendedSystematics = registry.recommendedSystematics()
    return recommendedSystematics

def doSystematics(algsToRun, nonSystAlgs = ["basicEventSelection", "mcEventVeto"] ) :
    '''This function will get the list of systematics from an initialized SUSYTools instance.  For each algorithm in algsToRun, it will add a copy of that algorithm to the algsToRun with an additional string for the systematic.  It will apply the systematic to those algorithms which have systName as a member of the class.  By default, we will skip running systematics on the algorithms : ''' , nonSystAlgs ,  '''.'''
    tmpAlgsToRun = copy(algsToRun)

    for syst in getSystList() :
        for algname, alg in algsToRun.iteritems() :
            if algname not in nonSystAlgs :
                tmpAlgsToRun[algname + '_' + syst.name() ] = deepcopy(alg)
                if hasattr(alg,"systName") :
                    setattr(alg, "systName" , syst.name())

    algsToRun.update(tmpAlgsToRun)
