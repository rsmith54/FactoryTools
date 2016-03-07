import os
import logging
import shutil
import ROOT
import basicEventSelectionConfig

from optparse import OptionParser

from datetime import date

def parseCommonOptions() :
    parser = OptionParser()
    parser.add_option("--submitDir", help   = "dir to store the output", default="submit_dir")
    parser.add_option("--inputDS", help     = "You can pass either the directory locally, the file containing the list of grid datasets, or directly the name of a grid dataset. ", default="/afs/cern.ch/work/r/rsmith/lvlv_datasets/")

    parser.add_option("--gridUser", help    = "gridUser"  , default=os.environ.get("USER")  )
    parser.add_option("--gridTag", help     = "gridTag", default=date.today().strftime("%m%d%y"))

    parser.add_option("--driver", help      = "select where to run", choices=("direct", "prooflite", "LSF","grid"), default="direct")
    parser.add_option('--doOverwrite', help = "Overwrite submit dir if it already exists",action="store_true", default=False)
    parser.add_option('--nevents', help     = "Run n events ", default = -1 )
    parser.add_option('--verbosity', help   = "Run all algs at the selected verbosity.",choices=("info", "warning","error", "debug", "verbose"), default="error")
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

def fillSampleHandler ( sh_all, inp , gridUser = os.environ.get("USER") , gridTag = date.today().strftime("%m%d%y")) :
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

def submitJob (job , driverName , submitDir, gridUser = os.environ.get("USER") , gridTag = date.today().strftime("%m%d%y")) :
    logging.info("creating driver")
    driver = None
    if (driverName == "direct"):
        logging.info( "direct driver")
        driver = ROOT.EL.DirectDriver()
        logging.info("submit job")
        driver.submit(job, submitDir)
    elif (driverName == "prooflite"):
        logging.info( "prooflite")
        driver = ROOT.EL.ProofDriver()
        logging.info("submit job")
        driver.submit(job, submitDir)
    elif (driverName == "grid"):
        logging.info( "grid driver")
        driver = ROOT.EL.PrunDriver()
        driver.options().setString("nc_outputSampleName", "user.%s.%%in:name[2]%%.%%in:name[3]%%.%s"%(gridUser,gridTag)   );
        driver.options().setDouble(ROOT.EL.Job.optGridMergeOutput, 1);

        logging.info("submit job")
        driver.submitOnly(job, submitDir)
    else :
        logging.info( "you gave an illegal driver name.  Not submitting.")


def configBasicEventSelection(alg , configDict = basicEventSelectionConfig.basicEventSelectionDict ) :
    #todo add ability to change individual pieces of the dictionary
    if os.path.islink('xAH_config.py') :
        os.unlink('xAH_config.py')
    os.symlink('$ROOTCOREBIN/user_scripts/xAODHelpers/xAH_config.py', 'xAH_config.py')

    from xAH_config import xAH_config


    c = xAH_config()
    c.setalg("BasicEventSelection", configDict)

