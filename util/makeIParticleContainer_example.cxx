#ifdef ROOTCORE
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// FrameWork includes
#include "AsgTools/ToolHandle.h"
#include "AsgTools/AsgTool.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODParticleEvent/ParticleAuxContainer.h"

#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingETAssociationMap.h"
#include "xAODMissingET/MissingETContainer.h"

#include "xAODCore/ShallowAuxContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODJet/JetContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJetContainer.h"

#include "assert.h"
#include "TFile.h"

#include "JetCalibTools/JetCalibrationTool.h"

#include "METUtilities/METSystematicsTool.h"
#include "METUtilities/METMaker.h"
#include "METUtilities/CutsMETMaker.h"

#include "PATInterfaces/SystematicRegistry.h"
#include "xAODParticleEvent/Particle.h"
#include "xAODMuon/Muon.h"
#include "xAODBase/IParticleHelpers.h"

int main(){std::cout << __PRETTY_FUNCTION__ << std::endl;
  xAOD::Init() ;

  //enable status code failures
  CP::CorrectionCode::enableFailure();
  CP::SystematicCode::enableFailure();
  StatusCode::enableFailure();
  xAOD::TReturnCode::enableFailure();

  xAOD::Muon *  muon = new xAOD::Muon;
  muon->makePrivateStore();
  muon->setP4(1e6 , 0 , 1.3);

  xAOD::Electron * electron = new xAOD::Electron();
  electron->makePrivateStore();
  electron->setP4(1.5e6, 0 , 2.6, .511e-3);


  std::pair<xAOD::IParticleContainer* , xAOD::ParticleAuxContainer* > containerpair =
    {new xAOD::IParticleContainer , new xAOD::ParticleAuxContainer} ;
  containerpair.first->setStore(containerpair.second);

  containerpair.first->push_back(muon);
  containerpair.first->push_back(electron);

  for (auto part : *containerpair.first) {
    std::cout << "part pT  : " << part->pt () << std::endl;
    std::cout << "part eta : " << part->eta() << std::endl;
    std::cout << "part phi : " << part->phi() << std::endl;
    std::cout << "part m   : " << part->m  () << std::endl;
    std::cout << "part E   : " << part->e  () << std::endl;

  }

  return 0;
 }



#endif
