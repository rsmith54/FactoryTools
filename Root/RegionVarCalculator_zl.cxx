#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "FactoryTools/RegionVarCalculator_zl.h"
#include "FactoryTools/strongErrorCheck.h"

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

EL::StatusCode RegionVarCalculator_zl::doCalculate(std::map<std::string, double              >& RegionVars,
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

  else if ( regionName == "CR2L") {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
               doCR2LCalculations(RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}

  else if ( regionName == "CRY") {return EL::StatusCode(doAllCalculations (RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS &&
               doCRYCalculations(RegionVars, VecRegionVars) == EL::StatusCode::SUCCESS);}


  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_zl::doAllCalculations(std::map<std::string, double>& RegionVars,
							 std::map<std::string, std::vector<double> > & VecRegionVars)
{/*todo*/
  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();

  doGeneralCalculations(RegionVars, VecRegionVars);

  // Get relevant info from the vertex container //////////////////////
  //

  const xAOD::VertexContainer* vertices = nullptr;
  STRONG_CHECK(event->retrieve( vertices, "PrimaryVertices"));
  RegionVars["NPV"] = HelperFunctions::countPrimaryVertices(vertices, 2);

  //
  /////////////////////////////////////////////////////////////////////

  auto toGeV = [](float a){return a*.001;};

  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  //  std::cout << "MET : " << (*metcont)["Final"]->met() << std::endl;
  RegionVars     ["MET"]   = toGeV((*metcont)["Final"]->met());

  // xAOD::JetContainer* jets_nominal(nullptr);
  // STRONG_CHECK(store->retrieve(jets_nominal, "STCalibAntiKt4EMTopoJets"));

  xAOD::IParticleContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal, "selectedJets"));

  //  const std::vector<xAOD::IParticle*> & jetStdVec = jetcont->stdcont();
  std::vector<double> jetPtVec;
  std::vector<double> jetEtaVec;
  std::vector<double> jetPhiVec;
  std::vector<double> jetEVec;

  for( const auto& jet : *jets_nominal) {
    jetPtVec.push_back( toGeV(jet->pt()));
    jetEtaVec.push_back( jet->p4().Eta() );
    jetPhiVec.push_back( jet->p4().Phi() );
    jetEVec.push_back( toGeV(jet->p4().E()) );
  }

  VecRegionVars[ "jetPt" ]  = jetPtVec;
  VecRegionVars[ "jetEta" ] = jetEtaVec;
  VecRegionVars[ "jetPhi" ] = jetPhiVec;
  VecRegionVars[ "jetE" ]   = jetEVec;

  xAOD::IParticleContainer* leptons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(leptons_nominal, "selectedLeptons"));

  std::vector<double> lepPtVec;
  std::vector<double> lepEtaVec;
  std::vector<double> lepPhiVec;
  std::vector<double> lepEVec;

  for( const auto& lep : *leptons_nominal) {
    lepPtVec.push_back( toGeV(lep->pt()));
    lepEtaVec.push_back( lep->p4().Eta() );
    lepPhiVec.push_back( lep->p4().Phi() );
    lepEVec.push_back( toGeV(lep->p4().E()) );
  }

  VecRegionVars[ "lepPt" ]  = lepPtVec;
  VecRegionVars[ "lepEta" ] = lepEtaVec;
  VecRegionVars[ "lepPhi" ] = lepPhiVec;
  VecRegionVars[ "lepE" ]   = lepEVec;



  double MEff = 0;
  double HT = 0;

  for( const auto& jet : *jets_nominal) {
    HT += toGeV(jet->pt());
  }

  MEff = HT + toGeV((*metcont)["Final"]->met());

  RegionVars["MEff"] = MEff;
  RegionVars["HT"] = HT;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_zl::doSRCalculations(std::map<std::string, double>& RegionVars,
							  std::map<std::string, std::vector<double> > & VecRegionVars)
{
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_zl::doCR1LCalculations(std::map<std::string, double>& RegionVars,
							    std::map<std::string, std::vector<double> > & VecRegionVars)
{
  auto toGeV = [](float a){return a*.001;};


  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();


  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  xAOD::IParticleContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal, "selectedJets"));

  xAOD::IParticleContainer* leptons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(leptons_nominal, "selectedLeptons"));

  double MEff = 0;
  double HT = 0;

  for( const auto& jet : *jets_nominal) {
    HT += toGeV(jet->pt());
  }

  for( const auto& lepton : *leptons_nominal) {
    HT += toGeV(lepton->pt());
  }

  MEff = HT + toGeV((*metcont)["Final"]->met());

  RegionVars["MEff"] = MEff;
  RegionVars["HT"] = HT;


  // double mT = std::sqrt( 2.*(*leptons_nominal)[0].p4().Pt()*(*metcont)["Final"]->met() *
  //                        (1.-((*leptons_nominal)[0].p4().Px()*(*metcont)["Final"]->mpx() + (*leptons_nominal)[0].p4().Py()*(*metcont)["Final"]->mpy())/((*leptons_nominal)[0].p4().Pt()*(*metcont)["Final"]->met())) );
  // if(!(mt >30000 && mt<100000)) return true
  // RegionVars["mT"] = mT;
  // Once mT is calculated, still need to cut on it in a post-selection!

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode RegionVarCalculator_zl::doCR2LCalculations(std::map<std::string, double>& RegionVars,
                  std::map<std::string, std::vector<double> > & VecRegionVars)
{
  auto toGeV = [](float a){return a*.001;};


  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();

  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  xAOD::IParticleContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal, "selectedJets"));

  xAOD::IParticleContainer* leptons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(leptons_nominal, "selectedLeptons"));

  double MEff = 0;
  double HT = 0;
  double MET = 0;

  xAOD::MissingET METVec(*(*metcont)["Final"]);


  for( const auto& lepton : *leptons_nominal) {
    METVec.add(lepton);
  }


  for( const auto& jet : *jets_nominal) {
    HT += toGeV(jet->pt());
  }

  MET = toGeV(  METVec.met() );
  MEff = HT + MET;

  RegionVars["MEff"] = MEff;
  RegionVars["MET"] = MET;

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode RegionVarCalculator_zl::doCRYCalculations(std::map<std::string, double>& RegionVars,
                  std::map<std::string, std::vector<double> > & VecRegionVars)
{
  auto toGeV = [](float a){return a*.001;};

  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();

  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  xAOD::IParticleContainer* photons_nominal(nullptr);
  STRONG_CHECK(store->retrieve(photons_nominal, "selectedPhotons"));


  std::vector<double> phPtVec;
  std::vector<double> phEtaVec;
  std::vector<double> phPhiVec;
  std::vector<double> phEVec;

  for( const auto& ph : *photons_nominal) {
    phPtVec.push_back( toGeV(ph->pt()));
    phEtaVec.push_back( ph->p4().Eta() );
    phPhiVec.push_back( ph->p4().Phi() );
    phEVec.push_back( toGeV(ph->p4().E()) );
  }

  VecRegionVars[ "phPt" ]  = phPtVec;
  VecRegionVars[ "phEta" ] = phEtaVec;
  VecRegionVars[ "phPhi" ] = phPhiVec;
  VecRegionVars[ "phE" ]   = phEVec;


  double MEff = 0;
  double MET = 0;

  xAOD::MissingET METVec(*(*metcont)["Final"]);

  // for( const auto& photon : *photons_nominal) {
  //   METVec.add(photon);
  // }

  METVec.add((*photons_nominal)[0]);

  MEff = toGeV(  METVec.met() );
  MEff = RegionVars["HT"] + MET;

  RegionVars["MEff"] = MEff;
  RegionVars["MET"] = MET;

  return EL::StatusCode::SUCCESS;

}
