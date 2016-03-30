#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"
#include "xAODRootAccess/TStore.h"

#include "SUSYTools/SUSYObjDef_xAOD.h"
//#include "xAODParticleEvent/ParticleContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "RJigsawTools/RegionVarCalculator_b4j.h"
#include "RJigsawTools/strongErrorCheck.h"

#include <xAODAnaHelpers/HelperFunctions.h>


// this is needed to distribute the algorithm to the workers
ClassImp(RegionVarCalculator_b4j)

EL::StatusCode RegionVarCalculator_b4j::doInitialize(EL::Worker * worker) {
  if(m_worker != nullptr){
    std::cout << "You have called " << __PRETTY_FUNCTION__ << " more than once.  Exiting." << std::endl;
    return EL::StatusCode::FAILURE;
  }
  m_worker = worker;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RegionVarCalculator_b4j::doCalculate(std::map<std::string, double              >& RegionVars,
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


  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_b4j::doAllCalculations(std::map<std::string, double>& RegionVars,
							   std::map<std::string, std::vector<double> > & VecRegionVars)
{
  xAOD::TStore * store = m_worker->xaodStore();
  xAOD::TEvent * event = m_worker->xaodEvent();

  // Get relevant info from the EventInfo object /////////////////////
  //

  const xAOD::EventInfo* eventInfo = nullptr;
  STRONG_CHECK(event->retrieve( eventInfo, "EventInfo"));

  doGeneralCalculations(RegionVars, VecRegionVars);

  //
  /////////////////////////////////////////////////////////////////////

  // Get relevant info from the vertex container //////////////////////
  //
  auto toGeV = [](float a){return a*.001;};


  const xAOD::VertexContainer* vertices = nullptr;
  STRONG_CHECK(event->retrieve( vertices, "PrimaryVertices"));
  RegionVars["NPV"] = HelperFunctions::countPrimaryVertices(vertices, 2);

  //
  /////////////////////////////////////////////////////////////////////

  xAOD::MissingETContainer * metcont = nullptr;
  STRONG_CHECK(store->retrieve(metcont, "STCalibMET"));

  //  std::cout << "MET : " << (*metcont)["Final"]->met() << std::endl;
  RegionVars     ["met"]   = toGeV((*metcont)["Final"]->met());

  // xAOD::JetContainer* jets_nominal(nullptr);
  // STRONG_CHECK(store->retrieve(jets_nominal, "STCalibAntiKt4EMTopoJets"));

  xAOD::JetContainer* jets_nominal(nullptr);
  STRONG_CHECK(store->retrieve(jets_nominal,  "STCalibCamKt12LCTopoJets"));


  std::vector<TLorentzVector> jet4MomVec;
  for( const auto& jet : *jets_nominal) {
    jet4MomVec.push_back( TLorentzVector(jet->p4()) );
  }
  auto ptSort = [](TLorentzVector const & a , TLorentzVector const & b){return a.Pt() > b.Pt();};
  std::sort(jet4MomVec.begin(),jet4MomVec.end(), ptSort);

  double mJJ = -1;
  if(jet4MomVec.size()>1){
    mJJ = (jet4MomVec.at(0)+jet4MomVec.at(1)).M();
  }

  RegionVars["mJJ"] = toGeV(mJJ);



  //  const std::vector<xAOD::IParticle*> & jetStdVec = jetcont->stdcont();
  std::vector<double> jetPtVec;
  std::vector<double> jetEtaVec;
  std::vector<double> jetPhiVec;
  std::vector<double> jetEVec;

  std::vector<double> jetTau1Vec;
  std::vector<double> jetTau2Vec;
  std::vector<double> jetTau3Vec;
  std::vector<double> jetDip12Vec;

  for( const auto& jet : *jets_nominal) {
    jetPtVec.push_back( toGeV(jet->pt()));
    jetEtaVec.push_back( jet->p4().Eta() );
    jetPhiVec.push_back( jet->p4().Phi() );
    jetEVec.push_back( toGeV(jet->p4().E()) );

    jetTau1Vec.push_back(  jet->getAttribute<double>("Tau1")    );
    jetTau2Vec.push_back(  jet->getAttribute<double>("Tau2")    );
    jetTau3Vec.push_back(  jet->getAttribute<double>("Tau3")    );
    jetDip12Vec.push_back( jet->getAttribute<double>("Dip12")     );
  }

  VecRegionVars[ "jetPt" ]  = jetPtVec;
  VecRegionVars[ "jetEta" ] = jetEtaVec;
  VecRegionVars[ "jetPhi" ] = jetPhiVec;
  VecRegionVars[ "jetE" ]   = jetEVec;

  VecRegionVars[ "jetTau1" ] = jetTau1Vec;
  VecRegionVars[ "jetTau2" ] = jetTau2Vec;
  VecRegionVars[ "jetTau3" ] = jetTau3Vec;
  VecRegionVars[ "jetDip12" ] = jetDip12Vec;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode RegionVarCalculator_b4j::doSRCalculations(std::map<std::string, double>& RegionVars,
							  std::map<std::string, std::vector<double> > & VecRegionVars)
{
  return EL::StatusCode::SUCCESS;
}

