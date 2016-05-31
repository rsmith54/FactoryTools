# TODO: 
#  - Additional calibration dicts for large-R jets
#  - Cross-check treatment of truth jets

#%%%%%%%%%%%%%%%%%%%%%%%%%% JetCalibrator %%%%%%%%%%%%%%%%%%%%%%%%%%#
jetCalibrationDict = {
  "m_name"		      :   "JetCalibrator",
  #----------------------- Container Flow ----------------------------#
  "m_inContainerName"         :   "AntiKt4EMTopoJets",
  "m_jetAlgo"                 :   "AntiKt4EMTopo",
  "m_outContainerName"        :   "AntiKt4EMTopoJets_Calib",
  "m_outputAlgo"              :   "AntiKt4EMTopoJets_Calib_Algo",
  "m_sort"                    :   True,
  "m_redoJVT"                 :   True,
  #----------------------- Systematics ----------------------------#
  #"m_systName"                :   "All",                 ## For MC
  #"m_systValVectorString"     :   "0.5,1,1.5,2,2.5,3.0", ## For MC signal samples
  #"m_systVal"                 :   1,                    ## For MC background samples
  "m_systName"                :   "Nominal",            ## For data
  "m_systVal"                 :   0,                    ## For data
  #----------------------- Calibration ----------------------------#
  "m_calibConfigAFII"         :   "JES_MC15Prerecommendation_AFII_June2015.config",
  "m_calibConfigFullSim"      :   "JES_MC15cRecommendation_May2016.config",
  "m_calibConfigData"         :   "JES_MC15cRecommendation_May2016.config",
  #JetCalibrator adds _Insitu for the data
  "m_calibSequence"           :   "JetArea_Residual_Origin_EtaJES_GSC",
  #----------------------- JES Uncertainty ----------------------------#
  "m_JESUncertConfig"         :   "$ROOTCOREBIN/data/JetUncertainties/JES_2015/ICHEP2016/JES2015_SR_Scenario1.config",
  ### For Fullsim ###
  "m_JESUncertMCType"         :   "MC15",
  ### For AFII ###
  #m_JESUncertMCType           :   "AFII",
  #m_setAFII                   :   True,
  #----------------------- JER Uncertainty ----------------------------#
  "m_JERUncertConfig"         :   "JetResolution/Prerec2015_xCalib_2012JER_ReducedTo9NP_Plots_v2.root",
  "m_JERFullSys"              :   False,
  "m_JERApplyNominal"         :   False,
  #----------------------- Cleaning ----------------------------#
  "m_jetCleanCutLevel"        :   "LooseBad",
  "m_jetCleanUgly"            :   False,
  "m_saveAllCleanDecisions"   :   True,
  "m_cleanParent"             :   False,
  #----------------------- Other ----------------------------#
  "m_debug"                   :   False

  }

#%%%%%%%%%%%%%%%%%%%%%%%%%% JetSelector %%%%%%%%%%%%%%%%%%%%%%%%%%#
jetSelectionDict = {
  "m_name"		      :   "JetSelector",
  #----------------------- Container Flow ----------------------------#
  "m_inContainerName"         :   "AntiKt4EMTopoJets_Calib",
  "m_outContainerName"        :   "SignalJets",
  "m_inputAlgo"               :   "AntiKt4EMTopoJets_Calib_Algo",
  "m_outputAlgo"              :   "SignalJets_Algo",
  "m_decorateSelectedObjects" :   True,
  "m_createSelectedContainer" :   True,
  #----------------------- Selections ----------------------------#
  ### max/min selections apply to m_pT, m_eta, m_detEta, m_mass, m_rapidity
  "m_cleanJets"               :   True,
  "m_pass_min"                :   4,
  "m_pT_min"                  :   50e3,
  "m_eta_max"                 :   2.5,
  #----------------------- JVT ----------------------------#
  "m_doJVT"                   :   True,
  "m_pt_max_JVT"              :   60e3,
  "m_eta_max_JVT"             :   2.4,
  "m_JVTCut"                  :   0.59,
  #----------------------- Other ----------------------------#
  ### Require jets pass truth label of (0,1,2,3,21) or 4 or 5
  #"m_truthLabel"              :   5,
  ### Choose the scale at which eta is checked
  #"m_jetScaleType"            :   "JetEMScaleMomentum",
  ### Only check the first N jets
  #"m_nToProcess"              :   2,
  ### Require the first N jets to be clean, otherwise fail event
  "m_cleanEvtLeadJets"        :   4,
  "m_debug"                   :   False,
  "m_useCutFlow"	      :   False

  }

#%%%%%%%%%%%%%%%%%%%%%%%%%% JetSelector: truth %%%%%%%%%%%%%%%%%%%%%%%%%%#
jetSelectionDict_truth = {
  "m_name"		      :   "JetSelector",
  #----------------------- Container Flow ----------------------------#
  "m_inContainerName"         :   "AntiKt4TruthJets",
  "m_outContainerName"        :   "SignalJets",
  "m_inputAlgo"               :   "AntiKt4TruthJets_Algo",
  "m_outputAlgo"              :   "SignalJets_Algo",
  "m_decorateSelectedObjects" :   True,
  "m_createSelectedContainer" :   True,
  #----------------------- Selections ----------------------------#
  ### max/min selections apply to m_pT, m_eta, m_detEta, m_mass, m_rapidity
  "m_cleanJets"               :   False,
  "m_pass_min"                :   4,
  "m_pT_min"                  :   50e3,
  "m_eta_max"                 :   2.5,
  #----------------------- JVT ----------------------------#
  "m_doJVT"                   :   True,
  "m_pt_max_JVT"              :   50e3,
  "m_eta_max_JVT"             :   2.4,
  "m_JVTCut"                  :   0.64,
  #----------------------- Other ----------------------------#
  ### Require jets pass truth label of (0,1,2,3,21) or 4 or 5
  #"m_truthLabel"              :   5,
  ### Choose the scale at which eta is checked
  #"m_jetScaleType"            :   "JetEMScaleMomentum",
  ### Only check the first N jets
  #"m_nToProcess"              :   2,
  ### Require the first N jets to be clean, otherwise fail event
  #"m_cleanEvtLeadJets"        :   4,
  "m_debug"                   :   False

}
