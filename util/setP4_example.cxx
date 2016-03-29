#ifdef ROOTCORE
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// FrameWork includes
#include "AsgTools/ToolHandle.h"
#include "AsgTools/AsgTool.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticleHelpers.h"

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

int main(){std::cout << __PRETTY_FUNCTION__ << std::endl;
  xAOD::Init() ;

  //enable status code failures
  CP::CorrectionCode::enableFailure();
  CP::SystematicCode::enableFailure();
  StatusCode::enableFailure();
  xAOD::TReturnCode::enableFailure();

  xAOD::Muon muon;
  muon.makePrivateStore();
  muon.setP4(1e6 , 0 , 1.3);

  std::cout << std::setprecision(12);

  std::cout << "muon pT  : " << muon.pt () << std::endl;
  std::cout << "muon eta : " << muon.eta() << std::endl;
  std::cout << "muon phi : " << muon.phi() << std::endl;
  std::cout << "muon m   : " << muon.m  () << std::endl;
  std::cout << "muon E   : " << muon.e  () << std::endl;

  std::cout << "muon p4 pT  : " << muon.p4().Pt () << std::endl;
  std::cout << "muon p4 eta : " << muon.p4().Eta() << std::endl;
  std::cout << "muon p4 phi : " << muon.p4().Phi() << std::endl;
  std::cout << "muon p4 m   : " << muon.p4().M  () << std::endl;
  std::cout << "muon p4 E   : " << muon.p4().E  () << std::endl;


  xAOD::Particle particlemuon;
  particlemuon.makePrivateStore();
  particlemuon.setP4( muon.p4());

  std::cout << "particlemuon pT  : " << particlemuon.pt () << std::endl;
  std::cout << "particlemuon eta : " << particlemuon.eta() << std::endl;
  std::cout << "particlemuon phi : " << particlemuon.phi() << std::endl;
  std::cout << "particlemuon m   : " << particlemuon.m  () << std::endl;
  std::cout << "particlemuon E   : " << particlemuon.e  () << std::endl;

  std::cout << "particlemuon p4 pT  : " << particlemuon.p4().Pt () << std::endl;
  std::cout << "particlemuon p4 eta : " << particlemuon.p4().Eta() << std::endl;
  std::cout << "particlemuon p4 phi : " << particlemuon.p4().Phi() << std::endl;
  std::cout << "particlemuon p4 m   : " << particlemuon.p4().M  () << std::endl;
  std::cout << "particlemuon p4 E   : " << particlemuon.p4().E  () << std::endl;




  return 0;
 }



#endif
