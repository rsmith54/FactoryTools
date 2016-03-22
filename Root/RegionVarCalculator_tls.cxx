#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "xAODParticleEvent/ParticleContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "RJigsawTools/RegionVarCalculator_tls.h"
#include "RJigsawTools/strongErrorCheck.h"

#include <xAODAnaHelpers/HelperFunctions.h>


// this is needed to distribute the algorithm to the workers
ClassImp(RegionVarCalculator_tls)

EL::StatusCode RegionVarCalculator_tls::doInitialize(EL::Worker * worker) {
  if(m_worker != nullptr){
    std::cout << "You have called " << __PRETTY_FUNCTION__ << " more than once.  Exiting." << std::endl;
    return EL::StatusCode::FAILURE;
  }
  m_worker = worker;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_tls::doCalculate(std::map<std::string, double              >& RegionVars,
						     std::map<std::string, std::vector<double> >& VecRegionVars){
  xAOD::TStore * store = m_worker->xaodStore();//grab the store from the worker
  xAOD::TEvent* event = m_worker->xaodEvent();

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

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


EL::StatusCode RegionVarCalculator_tls::doAllCalculations(std::map<std::string, double>& RegionVars,
							   std::map<std::string, std::vector<double> > & VecRegionVars)
{
  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();

  doGeneralCalculations(RegionVars, VecRegionVars);

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

  //  std::cout << "MET : " << (*metcont)["Final"]->met() << std::endl;
  RegionVars     ["met"]   = (*metcont)["Final"]->met();

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

  xAOD::ParticleContainer* leptons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(leptons_nominal, "selectedLeptons"));

  std::vector<double> lepPtVec;
  std::vector<double> lepEtaVec;
  std::vector<double> lepPhiVec;
  std::vector<double> lepEVec;
  std::vector<double> lepPdgidVec;

  for( const auto& lep : *leptons_nominal) {
    lepPtVec.push_back( lep->pt());
    lepEtaVec.push_back( lep->p4().Eta() );
    lepPhiVec.push_back( lep->p4().Phi() );
    lepEVec.push_back( lep->p4().E() );
    lepPdgidVec.push_back( lep->pdgId() );
  }

  VecRegionVars[ "lepPt" ]  = lepPtVec;
  VecRegionVars[ "lepEta" ] = lepEtaVec;
  VecRegionVars[ "lepPhi" ] = lepPhiVec;
  VecRegionVars[ "lepE" ]   = lepEVec;
  VecRegionVars[ "lepPdgidVec" ]   = lepPdgidVec;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_tls::doSRCalculations(std::map<std::string, double>& RegionVars,
							  std::map<std::string, std::vector<double> > & VecRegionVars)
{

  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::ParticleContainer* leptons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(leptons_nominal, "selectedLeptons"));

  // If we go to a >= 2 lepton SR, we're going to need to sort these collections but we will want to be
  // smart and careful about it given the way collections are handled in the store
  // auto ptSort = [](xAOD::Particle const & a , xAOD::Particle const & b){return a.pt() > b.pt();};
  // std::sort(leptons_nominal->begin(),leptons_nominal->end(), ptSort);
  // assert(leptons_nominal->at(0)->pt() > leptons_nominal->at(1)->pt());

  RegionVars[ "isSS" ]  = leptons_nominal->at(0)->pdgId()*leptons_nominal->at(1)->pdgId() > 0;
  RegionVars[ "isSF" ]  = abs(leptons_nominal->at(0)->pdgId()) == abs(leptons_nominal->at(1)->pdgId());

  RegionVars[ "mll" ]  = (leptons_nominal->at(0)->p4() + leptons_nominal->at(1)->p4() ).M();
  RegionVars[ "ptll" ]  = (leptons_nominal->at(0)->p4() + leptons_nominal->at(1)->p4() ).Pt();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode RegionVarCalculator_tls::doCR1LCalculations(std::map<std::string, double>& RegionVars,
							    std::map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}


EL::StatusCode RegionVarCalculator_tls::doCR0LCalculations(std::map<std::string, double>& RegionVars,
							    std::map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/return EL::StatusCode::SUCCESS;}
