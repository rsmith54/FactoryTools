#!/usr/bin/env python

########### Initialization ######################################
##
##

import ROOT
import logging
import shutil
import os, sys
import itertools
import array

import discoverInput

logging.basicConfig(level=logging.INFO)
from optparse import OptionParser

import atexit
@atexit.register
def quite_exit():
	ROOT.gSystem.Exit(0)

# from multiprocessing import Pool
import multiprocessing as mp


logging.info("loading packages...")
ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

##
##
########### Configuration ######################################
##
##


parser = OptionParser()
parser.add_option("--inDir", help   = "dir with output", default="/afs/cern.ch/user/l/leejr/work/public/fromGrid/")
parser.add_option("--nproc", help     = "number of parallel processes", default="4"  )
parser.add_option("--selection", help     = "selection string for skimming", default="1"  )

(options, args) = parser.parse_args()

# lumi = 1000  ## in pb-1
# if len(sys.argv)==1:
# 	search_directories = ["/afs/cern.ch/user/l/leejr/work/public/fromGrid/",]
# else:
# 	search_directories = [str(sys.argv[i]) for i in xrange(1,len(sys.argv))]

print options
search_directories = [options.inDir]
print search_directories

tmpOutputDirectory = "tmpOutput"
outputDirectory = "output"
treeStream = "trees"
treePrefix = "trees_"

selection = options.selection
ncores = min(int(options.nproc),mp.cpu_count())


outputSampleNames = [
	"data",
	"signal",
	"qcd",
	"top",
	"wjets",
	"zjets",
	"diboson",
	"electroweak",
]


##
##
########### Gather input ######################################
##
##

logging.info("creating new sample handler")
sh_all = ROOT.SH.SampleHandler()

discoverInput.discover(sh_all, search_directories, "*%s*"%treeStream  )
print len(sh_all)

logging.info("adding my tags defined in discoverInput.py")
discoverInput.addTags(sh_all)

ROOT.SH.readSusyMetaDir(sh_all,"$ROOTCOREBIN/data/SUSYTools")
ROOT.SH.readSusyMetaDir(sh_all,"$ROOTCOREBIN/data/SUSYTools/mc15_13TeV/")




def main():

	if int(ncores)>1:
		pool = mp.Pool(processes=ncores)
		pool.map(processTheSH, outputSampleNames)
		pool.close()
		pool.join()
	else:
		for outputSampleName in outputSampleNames:
			processTheSH(outputSampleName)

	return


def processTheSH( SHname ):

	print SHname

	## Split up samplehandler into per-BG SH's based on tag metadata

	sh = sh_all.find(SHname)

	treesToProcess = []

	filesToEventuallyHadd = []

	for sample in sh:
		sample_name = sample.getMetaString("sample_name")
		# print sample_name
		dsid = sample_name.split(".")[2]

		if len(treesToProcess) == 0:
			treesToProcess = getListOfTreeNames(sample)

		attachCounters(sample)

		mydir = tmpOutputDirectory

		try:
			os.stat(mydir)
		except:
			os.mkdir(mydir)       

		outputSampleFileName = "%s/%s.root"%(tmpOutputDirectory, dsid)
		filesToEventuallyHadd.append(outputSampleFileName)

		outputSampleFile = ROOT.TFile(outputSampleFileName,"RECREATE")  

		print "Starting"
		print os.stat(outputSampleFileName).st_size 

		for itree in treesToProcess:
			sh.setMetaString("nc_tree", itree)
			outputSampleFile.cd()
			mytree =  sample.makeTChain().Clone(itree)
			print mytree
			print getNormFactor(sample)
			# print selection
			if mytree.GetEntries() and getNormFactor(sample):
				try:
					outputTree = ROOT.addBranch( mytree, getNormFactor(sample) , selection)
				except:
					continue
				print outputTree.GetEntries()
				outputTree.Write()
				print "Saved tree %s with %s events . . ." % ( outputTree.GetName(), outputTree.GetEntries() )
			print os.stat(outputSampleFileName).st_size 

		print os.stat(outputSampleFileName).st_size 
		print "WRITING FILE...."
		outputSampleFile.Write()
		print os.stat(outputSampleFileName).st_size 
		outputSampleFile.Close()
		print os.stat(outputSampleFileName).st_size 




	mydir = outputDirectory
	try:
		os.stat(mydir)
	except:
		os.mkdir(mydir)  
	try:
		os.stat(mydir+"/signal")
	except:
		os.mkdir(mydir+"/signal")  



	if SHname!="signal":
		os.system('hadd -O -f %s/%s.root %s'%
			(outputDirectory, SHname, " ".join(filesToEventuallyHadd) ) 
			)
	else:
		for myfile in filesToEventuallyHadd:
			os.system('cp %s %s/signal/.'% (myfile,outputDirectory)  )

	return




#To scale the histograms in the files after the event loop is done...
def getNormFactor(sample):

	tempxs = sample.getMetaDouble("nc_xs") * sample.getMetaDouble("kfactor") * sample.getMetaDouble("filter_efficiency")
	
	print "Norm weight for %s is %f/(%f or %f)"%(sample.getMetaString("short_name"), tempxs, sample.getMetaDouble("nc_nevt"), sample.getMetaDouble("nc_sumw"))
	m_eventscaling = tempxs
	if sample.getMetaDouble("nc_nevt"):
		m_eventscaling /= sample.getMetaDouble("nc_nevt") if "jetjet" in sample.getMetaString("short_name") else sample.getMetaDouble("nc_sumw")
	else:
		m_eventscaling = 0.
	return m_eventscaling


addBranchCode = """
TTree * addBranch(TTree* tree, float normalization, TString selection="1"){

		TTree * newtree = tree->CopyTree(selection);
		double normweight = normalization;
		TBranch * bnormweight = newtree->Branch("normweight",&normweight,"normweight/D");
		int nevents = newtree->GetEntries();

		for (Long64_t i=0;i<nevents;i++) {
			newtree->GetEntry(i);
			bnormweight->Fill();
			if(i%10000==0) cout<< i << " of " << nevents << endl;
		}

		return newtree;
}
"""

ROOT.gInterpreter.Declare(addBranchCode)

# This python function is replaced by the c++ function above for speed
# def addLeaves(tree,normalization,selection="1"):
# 	return 0
# 	events = tree.GetEntries()
# 	leaves = "normweight/D"
# 	leafValues = array.array("d", [0.])
# 	# newtree = tree.CloneTree(0)
# 	newtree = tree.CopyTree(selection)
# 	newBranch = newtree.Branch( "normweight" , leafValues, leaves )
# 	for i in xrange(events):
# 		tree.GetEntry(i)
# 		leafValues[0] = ROOT.Double(normalization)
# 		newtree.Fill()
# 		if i % 10000 == 0:
# 			print "%s of %s: %s" % (i,events,leafValues)
# 	return newtree


def attachCounters(sample):

		m_nevt = 0
		m_sumw = 0

		#Go to the grid and get the metadata output
		sh_metadata = ROOT.SH.SampleHandler()
		discoverInput.discover(sh_metadata, search_directories, sample.getMetaString("sample_name") )
		if len(sh_metadata) == 1:
			metadata_sample = sh_metadata[0]
			for myfile in [ROOT.TFile(ifilepath) for ifilepath in metadata_sample.makeFileList() ]:
				print myfile 
				try:
					m_nevt += myfile.Get("cutflow").GetBinContent(myfile.Get("cutflow").GetXaxis().FindBin("all"))
					m_sumw += myfile.Get("cutflow_weighted").GetBinContent(myfile.Get("cutflow_weighted").GetXaxis().FindBin("all"))
				except:
					pass

		sample.setMetaDouble("nc_nevt",m_nevt)
		sample.setMetaDouble("nc_sumw",m_sumw)



def getListOfTreeNames(sample):
	f = ROOT.TFile(sample.fileName(0) )
	# print f
	listOfTrees = [key.GetName() for key in f.GetListOfKeys() if treePrefix in key.GetName()]
	return listOfTrees




if __name__ == "__main__":
	main()

