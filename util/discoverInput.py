import ROOT
import logging
import shutil
import os


def discover(sh, search_directories, pattern="*trees*"):

	# scan for datasets in the given directories
	for directory in search_directories:
	    ROOT.SH.ScanDir().samplePattern(pattern).scan(sh, directory)


	logging.info("%d different datasets found scanning all directories", len(sh))

	return sh



def addTags(sh_all):
	for sample in sh_all:

		sample_name = sample.getMetaString("sample_name")
		short_name = sample.getMetaString("sample_name").split(".")[3]
		dsid = int(sample.getMetaString("sample_name").split(".")[2])
		sample.setMetaString( "short_name" , sample.getMetaString("sample_name") )

		print short_name

		if "physics_" in short_name:
			sample.addTag("data")

		#if "GG_direct" in short_name:
		#	sample.addTag("signal")
		#	sample.addTag("gg_direct")
		#if "SS_direct" in short_name:
		#	sample.addTag("signal")
		#	sample.addTag("ss_direct")


		if "SM_GG_N2" in short_name:
			sample.addTag("signal")
			sample.addTag("gluino")

		if "GG_onestepCC" in short_name:
			sample.addTag("signal")
			sample.addTag("gg_onestepcc")

		if "SS_onestepCC" in short_name:
			sample.addTag("signal")
			sample.addTag("ss_onestepcc")


		if "410000" in sample_name:
			sample.addTag("top")
			sample.addTag("ttbar")
		if dsid in range(410011,410015):
			sample.addTag("top")
			sample.addTag("singletop")
		if dsid in range(410066,410082):
			sample.addTag("top")
			sample.addTag("ttv")

		if "jetjet" in short_name:
			sample.addTag("qcd")

		# if "1Gam" in short_name:
		# 	sample.addTag("gamma")

		if "Sherpa_CT10_W" in short_name and "_Pt" in short_name:
			sample.addTag("wjets")
		if "Sherpa_CT10_Z" in short_name and "_Pt" in short_name:
			sample.addTag("zjets")
		# if "Znunu" in short_name:
		# 	sample.addTag("zjets")
		# if "Sherpa_Wqq" in short_name:
		# 	sample.addTag("zjets")
		# if "_Zmumu_" in short_name:
		# 	sample.addTag("zjets")
		# if "ggH" in short_name:
		# 	sample.addTag("signal")


		# if "PowhegPy8EG_CT10nloME_AZNLOCTEQ6L1_" in short_name and any(x in short_name for x in ["WW","ZZ","WZ"]):
			# sample.addTag("diboson")

		if dsid in range(361063,361088):
			sample.addTag("diboson")

		# if any(x in short_name for x in ["Sherpa_CT10_munugamma","Sherpa_CT10_nunugamma","Sherpa_CT10_ll","Sherpa_CT10_gg"]):
		# 	sample.addTag("electroweak")
