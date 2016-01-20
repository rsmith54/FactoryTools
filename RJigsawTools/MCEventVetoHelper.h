#ifndef RJigsawTools_MCEventVetoHelper_H_
#define RJigsawTools_MCEventVetoHelper_H_

#include "xAODJet/JetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

// -------------------------------------------------------------------
//  MCEventVetoHelper:
//    helper class for MCEventVeto to work around the limitations of
//    xAOD EDM vs CINT
// -------------------------------------------------------------------


class MCEventVetoHelper
{
 public:
  MCEventVetoHelper() {};

  static bool isHighPtDijet(const xAOD::JetContainer* jets);

  static bool isHighPtJetMET(uint32_t mc_channel_number, 
			     const xAOD::JetContainer* jets,
			     const xAOD::MissingETContainer* metc);


  // helper function for mc12accept
  static unsigned int vetoQEDFSR(uint32_t mc_channel_number, 
				 const xAOD::TruthParticleContainer* mcparticles);
  static bool trueBosonFromWorZplusJetsMCSample(TLorentzVector&trueBoson, 
						uint32_t mc_channel_number,
						const xAOD::TruthParticleContainer* mcparticles);

  // general function for truth veto on mc14 samples
  static bool mc15accept(
			 uint32_t mc_channel_number, 
			 const xAOD::TruthParticleContainer* mcparticles, 
			 const xAOD::MissingETContainer* metc);

  static bool mc15SherpaWZjets_accept(
				      uint32_t mc_channel_number, 
				      const xAOD::TruthParticleContainer* mcparticles);

 private:

};

#endif
