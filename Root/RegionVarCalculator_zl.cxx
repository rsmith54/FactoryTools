#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "xAODParticleEvent/ParticleContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "RJigsawTools/RegionVarCalculator_zl.h"
#include "RJigsawTools/strongErrorCheck.h"

#include <xAODAnaHelpers/HelperFunctions.h>


// this is needed to distribute the algorithm to the workers
ClassImp(RegionVarCalculator_zl)

EL::StatusCode RegionVarCalculator_zl::doInitialize(EL::Worker * worker) {
  if(m_worker != nullptr){
    std::cout << "You have called " << __PRETTY_FUNCTION__ << " more than once.  Exiting." << std::endl;
    return EL::StatusCode::FAILURE;
  }
  m_worker = worker;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_zl::doCalculate(std::unordered_map<std::string, double              >& RegionVars,
						     std::unordered_map<std::string, std::vector<double> >& VecRegionVars){
  xAOD::TStore * store = m_worker->xaodStore();//grab the store from the worker
  xAOD::TEvent * event = m_worker->xaodEvent();

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  std::string const & regionName = eventInfo->auxdecor< std::string >("regionName");

  if      ( regionName.empty() ) {return EL::StatusCode::SUCCESS;}
  // If it hasn't been selected in any of the regions from any of the select algs, don't bother calculating anything...
  else if ( regionName == "SR" )  {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
							 doSRCalculations  (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}
  else if ( regionName == "TriggerPassThrough") {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
							 doTriggerPassThroughCalculations(RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}


  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_zl::doAllCalculations(std::unordered_map<std::string, double>& RegionVars,
							   std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/
  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();

  //store->print();

  // Get relevant info from the EventInfo object /////////////////////
  //

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  RegionVars["runNumber"]   = eventInfo->runNumber();
  RegionVars["lumiBlock"]   = eventInfo->lumiBlock();
  RegionVars["bcid"]        = eventInfo->bcid();
  RegionVars["eventNumber"] = eventInfo->eventNumber();
  RegionVars["mcChannelNumber"] = eventInfo->mcChannelNumber();
  RegionVars["actualInteractionsPerCrossing"] = eventInfo->actualInteractionsPerCrossing();
  RegionVars["averageInteractionsPerCrossing"] = eventInfo->averageInteractionsPerCrossing();

  RegionVars["mcEventWeight"] = eventInfo->auxdecor< int >("mcEvtWeight");
  RegionVars["pileupWeight"] = eventInfo->auxdecor< float >("PileupWeight");

  //
  /////////////////////////////////////////////////////////////////////

  // Get relevant info from the vertex container //////////////////////
  //

  const xAOD::VertexContainer* vertices = nullptr;
  STRONG_CHECK(event->retrieve( vertices, "PrimaryVertices"));
  RegionVars["NPV"] = HelperFunctions::countPrimaryVertices(vertices, 2);

  //
  /////////////////////////////////////////////////////////////////////

  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  std::cout << "MET : " << (*metcont)[m_metName]->met() << std::endl;
  RegionVars     ["met"]   = (*metcont)[m_metName]->met();

  // xAOD::JetContainer* jets_nominal(nullptr);
  // STRONG_CHECK(store->retrieve(jets_nominal, "STCalibAntiKt4EMTopoJets"));

  xAOD::ParticleContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal, "selectedJets"));

  //  const std::vector<xAOD::IParticle*> & jetStdVec = jetcont->stdcont();
  std::vector<double> jetPtVec;
  std::vector<double> jetEtaVec;
  std::vector<double> jetPhiVec;
  std::vector<double> jetEVec;

  for( const auto& jet : *jets_nominal) {
    jetPtVec.push_back( jet->pt());
    jetEtaVec.push_back( jet->p4().Eta() );
    jetPhiVec.push_back( jet->p4().Phi() );
    jetEVec.push_back( jet->p4().E() );
  }

  VecRegionVars[ "jetPt" ]  = jetPtVec;
  VecRegionVars[ "jetEta" ] = jetEtaVec;
  VecRegionVars[ "jetPhi" ] = jetPhiVec;
  VecRegionVars[ "jetE" ]   = jetEVec;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_zl::doTriggerPassThroughCalculations(std::unordered_map<std::string, double>& RegionVars,
                std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{

// Put Trigger Decisions here
  xAOD::TEvent * event = m_worker->xaodEvent();
  xAOD::TStore * store = m_worker->xaodStore();

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  std::vector< std::string > const & passTrigs = eventInfo->auxdecor<  std::vector< std::string >  >("passTriggers");

  std::cout << "test" << std::endl;  
  std::cout << passTrigs.size() << std::endl;  
  if(passTrigs.size()){std::cout << passTrigs[0] << std::endl;  }
  

  RegionVars["pass_xe100"]              = std::find(passTrigs.begin(), passTrigs.end(), "HLT_xe100") != passTrigs.end();
  RegionVars["pass_xe70"]               = std::find(passTrigs.begin(), passTrigs.end(), "HLT_xe70") != passTrigs.end();
  RegionVars["pass_j30_xe60_razor170"]  = std::find(passTrigs.begin(), passTrigs.end(), "HLT_j30_xe60_razor170") != passTrigs.end();
  RegionVars["pass_j30_xe60_razor185"]  = std::find(passTrigs.begin(), passTrigs.end(), "HLT_j30_xe60_razor185") != passTrigs.end();
  RegionVars["pass_j30_xe60_razor195"]  = std::find(passTrigs.begin(), passTrigs.end(), "HLT_j30_xe60_razor195") != passTrigs.end();

  RegionVars["pass_2J15_XE55"]  = std::find(passTrigs.begin(), passTrigs.end(), "L1_2J15_XE55") != passTrigs.end();
  RegionVars["pass_mu20"]  = std::find(passTrigs.begin(), passTrigs.end(), "HLT_mu20") != passTrigs.end();



  return EL::StatusCode::SUCCESS;}

EL::StatusCode RegionVarCalculator_zl::doSRCalculations(std::unordered_map<std::string, double>& RegionVars,
                std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}


EL::StatusCode RegionVarCalculator_zl::doCR1LCalculations(std::unordered_map<std::string, double>& RegionVars,
							    std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}


EL::StatusCode RegionVarCalculator_zl::doCR0LCalculations(std::unordered_map<std::string, double>& RegionVars,
							    std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}
