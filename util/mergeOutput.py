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


logging.info("loading packages...")
ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

##
##
########### Configuration ######################################
##
##

lumi = 1000  ## in pb-1
if len(sys.argv)==1:
	search_directories = ["/afs/cern.ch/user/l/leejr/work/public/fromGrid/",]
else:
	search_directories = [str(sys.argv[i]) for i in xrange(1,len(sys.argv))]

print search_directories

tmpOutputDirectory = "tmpOutput"
outputDirectory = "output"
treePrefix = "trees"

selection = "1"

def main():

	##
	##
	########### Gather input ######################################
	##
	##

	logging.info("creating new sample handler")
	sh_all = ROOT.SH.SampleHandler()

	discoverInput.discover(sh_all, search_directories, "*%s*"%treePrefix  )
	print len(sh_all)

	logging.info("adding my tags defined in discoverInput.py")
	discoverInput.addTags(sh_all)

	## Split up samplehandler into per-BG SH's based on tag metadata

	sh_data = sh_all.find("data")
	sh_signal = sh_all.find("signal")
	sh_bg = {}

	sh_bg["qcd"] = sh_all.find("qcd")
	sh_bg["top"] = sh_all.find("top")
	sh_bg["wjets"] = sh_all.find("wjets")
	sh_bg["zjets"] = sh_all.find("zjets")

	######This will be done per samplehandler ############################
	##
	##

	#Creation of output directory names
	outputFileNames = {   
		sh_all: "All",
		sh_data: "Data",
		sh_signal: "Signal",
		sh_bg["qcd"]: "QCD",
		sh_bg["top"]: "Top",
		sh_bg["wjets"]: "WJets",
		sh_bg["zjets"]: "ZJets",
		}

	treesToProcess = []

	for mysamplehandler in [	
								sh_all,
								# sh_data,
								# sh_bg["qcd"],
								# sh_bg["top"],
								# sh_bg["wjets"],
								# sh_bg["zjets"] 
							]:

		print mysamplehandler

		filesToEventuallyHadd = []

		for sample in mysamplehandler:

			sample_name = sample.getMetaString("sample_name")
			print sample_name
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

			for itree in treesToProcess:
				mysamplehandler.setMetaString("nc_tree", itree)
				mytree =  sample.makeTChain().Clone()
				outputTree = addLeaves( mytree, getNormFactor(sample) , selection)
				outputTree.Write()

			print "Saved tree %s with %s events . . ." % ( outputTree.GetName(), outputTree.GetEntries() )

			outputSampleFile.Close()


		mydir = outputDirectory
		try:
			os.stat(mydir)
		except:
			os.mkdir(mydir)       
		os.system('hadd -f %s/%s.root %s'%
			(outputDirectory, outputFileNames[mysamplehandler], " ".join(filesToEventuallyHadd) ) 
			)



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



def addLeaves(tree,normalization,selection="1"):


	events = tree.GetEntries()

	leaves = "normweight/F"
	leafValues = array.array("d", [0.0])
	# newtree = tree.CloneTree(0)
	newtree = tree.CopyTree(selection)

	newBranch = newtree.Branch( "normweight" , leafValues, leaves )
	for i in range(events):
		tree.GetEntry(i)

		leafValues[0] = normalization

		newtree.Fill()

		if i % 5000 == 0:
			print "%s of %s: %s" % (i,events,leafValues)

	return newtree


def attachCounters(sample):

		m_nevt = 0
		m_sumw = 0

		#Go to the grid and get the metadata output
		sh_metadata = ROOT.SH.SampleHandler()
		discoverInput.discover(sh_metadata, search_directories, sample.getMetaString("sample_name").replace("trees","metadata") )
		assert len(sh_metadata) == 1
		metadata_sample = sh_metadata[0]
		for myfile in [ROOT.TFile(ifilepath) for ifilepath in metadata_sample.makeFileList() ]:
			print myfile 
			try:
				m_nevt += myfile.Get("MetaData_EventCount").GetBinContent(1)
				m_sumw += myfile.Get("MetaData_EventCount").GetBinContent(3)
			except:
				pass

		sample.setMetaDouble("nc_nevt",m_nevt)
		sample.setMetaDouble("nc_sumw",m_sumw)



def getListOfTreeNames(sample):
	f = ROOT.TFile(sample.fileName(0) )
	print f
	listOfTrees = [key.GetName() for key in f.GetListOfKeys() if treePrefix in key.GetName()]
	return listOfTrees




if __name__ == "__main__":
	main()

