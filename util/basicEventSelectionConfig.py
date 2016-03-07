from xAH_config import xAH_config

basicEventSelectionDict = {
    "m_applyGRLCut" : True,
    "m_GRLxml" : "$ROOTCOREBIN/data/xAODAnaHelpers/data15_13TeV.periodAllYear_HEAD_DQDefects-00-01-02_PHYS_StandardGRL_Atlas_Ready.xml",
    "m_cleanPowheg" : True,
    "m_doPUreweighting" : False,
    "m_lumiCalcFileNames" : "",
    "m_PRWFileNames" : "",
    "m_PU_default_channel" : 0,
    "m_vertexContainerName" : "PrimaryVertices",
    "m_applyPrimaryVertexCut" : True,
    "m_PVNTrack" : 2,
    "m_applyEventCleaningCut" : True,
    "m_applyCoreFlagsCut" : True,
    "m_triggerSelection" : "HLT_(e2|mu2).*",
    "m_applyTriggerCut" : True,
    "m_storeTrigDecisions" : True,
    "m_storePassL1" : False,
    "m_storePassHLT" : True,
    "m_storeTrigKeys" : False,
    "m_derivationName" : "SUSY2",
    "m_useMetaData" : False,
    "m_checkDuplicatesData" : True,
    "m_checkDuplicatesMC" : True,
}

def config(alg , configDict = basicEventSelctionDict ) :
    c = xAH_config()
    c.setalg("BasicEventSelection", configDict)

