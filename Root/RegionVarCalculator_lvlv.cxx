#include "EventLoop/StatusCode.h"
#include "xAODRootAccess/TStore.h"

#include "RJigsawTools/RegionVarCalculator_lvlv.h"

// this is needed to distribute the algorithm to the workers
ClassImp(RegionVarCalculator_lvlv)

EL::StatusCode RegionVarCalculator_lvlv::doInitialize(xAOD::TStore * store) {
  if(m_store != nullptr){
    std::cout << "You have called " << __PRETTY_FUNCTION__ << " more than once.  Exiting." << std::endl;
    return EL::StatusCode::FAILURE;}
  m_store = store;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_lvlv::doCalculate(std::unordered_map<std::string, double>& RegionVars,
						     xAOD::IParticleContainer& particles,
						     xAOD::MissingET& met
						     ){


  return EL::StatusCode::SUCCESS;
}
