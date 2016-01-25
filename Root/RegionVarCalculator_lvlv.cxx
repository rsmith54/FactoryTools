#include "EventLoop/StatusCode.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "xAODParticleEvent/ParticleContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "RJigsawTools/RegionVarCalculator_lvlv.h"
#include "RJigsawTools/strongErrorCheck.h"

// this is needed to distribute the algorithm to the workers
ClassImp(RegionVarCalculator_lvlv)

EL::StatusCode RegionVarCalculator_lvlv::doInitialize(xAOD::TStore * store) {
  if(m_store != nullptr){
    std::cout << "You have called " << __PRETTY_FUNCTION__ << " more than once.  Exiting." << std::endl;
    return EL::StatusCode::FAILURE;}
  m_store = store;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_lvlv::doCalculate(std::unordered_map<std::string, double              >& RegionVars,
						     std::unordered_map<std::string, std::vector<double> >& VecRegionVars){
  xAOD::TStore * store = m_store;//this is just to be able to copy code :)

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(store->retrieve( eventInfo, "EventInfo"));

  std::string const & regionName = eventInfo->auxdecor< std::string >("regionName");

  if      ( regionName.empty() ) {return EL::StatusCode::SUCCESS;}
  // If it hasn't been selected in any of the regions from any of the select algs, don't bother calculating anything...
  else if ( regionName == "SR" )  {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
							 doSRCalculations  (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}

  else if ( regionName == "CR1L") {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
							 doCR1LCalculations(RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}

  else if ( regionName == "CR0L") {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
							 doCR0LCalculations(RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}


  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_lvlv::doAllCalculations(std::unordered_map<std::string, double>& RegionVars,
							   std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/
  xAOD::TStore * store = m_store;

  //store->print();

  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  std::cout << "MET : " << (*metcont)["Final"]->met() << std::endl;
  RegionVars     ["met"]   = (*metcont)["Final"]->met();

  // xAOD::JetContainer* jets_nominal(nullptr);
  // STRONG_CHECK(store->retrieve(jets_nominal, "STCalibAntiKt4EMTopoJets"));

  xAOD::ParticleContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal, "selectedJets"));

  //  const std::vector<xAOD::IParticle*> & jetStdVec = jetcont->stdcont();
  std::vector<double> jetPtVec;

  for( const auto& jet : *jets_nominal) {
    jetPtVec.push_back( jet->pt());
  }

  VecRegionVars   [ "jetPt"] = jetPtVec;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_lvlv::doSRCalculations(std::unordered_map<std::string, double>& RegionVars,
							  std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}


EL::StatusCode RegionVarCalculator_lvlv::doCR1LCalculations(std::unordered_map<std::string, double>& RegionVars,
							    std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}


EL::StatusCode RegionVarCalculator_lvlv::doCR0LCalculations(std::unordered_map<std::string, double>& RegionVars,
							    std::unordered_map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}
