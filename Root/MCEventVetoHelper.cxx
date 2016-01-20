
#include "RJigsawTools/MCEventVetoHelper.h"

#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"

bool MCEventVetoHelper::isHighPtDijet(const xAOD::JetContainer* jets)
{

  float leadjetPt = -1.;
  float secondjetPt = -1.;
  for ( xAOD::JetContainer::const_iterator it = jets->begin();
	it != jets->end(); ++it ){
    double pt = (*it)->pt();
    if ( pt > leadjetPt ) {
      secondjetPt = leadjetPt;
      leadjetPt = pt;
    }
    else if ( pt > secondjetPt ) {
      secondjetPt = pt;
    }
  }
  //std::cout << " leading thruth jet Pts " <<  leadjetPt << " " << secondjetPt << std::endl;
  if ( secondjetPt < 100000. ) return false;

  return true;
}

bool MCEventVetoHelper::isHighPtJetMET(uint32_t mc_channel_number, const xAOD::JetContainer* jets, const xAOD::MissingETContainer* metc)
{
  if( mc_channel_number < 107680 ||  mc_channel_number > 107720) {
    return false;
  }

  xAOD::MissingETContainer::const_iterator truthmet_it = metc->find("NonInt");
  if ( truthmet_it == metc->end() ) {
    throw(std::runtime_error("Could not find Truth MET with name NonInt"));
  }
  //std::cout << " MET Truth_NonInt " <<  (*truthmet_it)->met() << std::endl;
  if ( (*truthmet_it)->met() < 100000. ) return false;

  float leadjetPt = -1.;
  for ( xAOD::JetContainer::const_iterator it = jets->begin();
	it != jets->end(); ++it ){
    double pt = (*it)->pt();
    if ( pt > leadjetPt ) {
      leadjetPt = pt;
    }
  }
  //std::cout << " leading thruth jet Pt " <<  leadjetPt << std::endl;
  if ( leadjetPt < 100000. ) return false;

  return true;
}

unsigned int MCEventVetoHelper::vetoQEDFSR(uint32_t mc_channel_number, const xAOD::TruthParticleContainer* mcparticles)
{
  // Remove overlap between Sherpa Zll+gamma (Wlv+gamma) and inclusive Z (W) samples
  // Veto events from the Z or W inclusive samples if a QED FSR is emitted with a large angle (>0.1)
  // with respect to the charged lepton.
  unsigned int veto = 0;
  bool isSherpaWZ = false;
  isSherpaWZ = isSherpaWZ || (mc_channel_number >= 147774 && mc_channel_number <= 147776); // Inclusive W
  isSherpaWZ = isSherpaWZ || (mc_channel_number >= 144992 && mc_channel_number <= 144994); // Sherpa W + >= 3jets
  isSherpaWZ = isSherpaWZ || (mc_channel_number >= 157534 && mc_channel_number <= 157536); // Sherpa W + PtW > 200 GeV
  isSherpaWZ = isSherpaWZ || (mc_channel_number >= 147770 && mc_channel_number <= 147772); // Inclusive Zll
  if (!isSherpaWZ) return veto;

  for ( xAOD::TruthParticleContainer::const_iterator it = mcparticles->begin();
	it != mcparticles->end(); ++it ){
    //std::cout << " MCparticle " << (*it)->pdgId() << " " << (*it)->barcode() << " " <<  (*it)->status() << " " << (*it)->pt() << std::endl;
    //FIXME is the order now such that we can break the loop in this case?
    if ( (*it)->barcode()  >= 200000) break; // GEANT particles, skip brem photons
    if ( (*it)->pdgId() != 22) continue;
    if ( (*it)->pt() < 10000.) continue;
    size_t nParents = 0;
    if ((*it)->hasProdVtx()) nParents = (*it)->prodVtx()->nIncomingParticles();
    //std::cout << " MCparticle " << (*it)->pdgId() << " " << (*it)->barcode() << (*it)->status() << " " << (*it)->pt() << " " << nParents << std::endl;
    if ( nParents < 2) continue;

    unsigned int nlep = 0;
    float dRmin = 999.;
    const xAOD::TruthVertex* originVertex = (*it)->prodVtx();
    for ( size_t i = 0; i < nParents; ++i ){
      const xAOD::TruthParticle* parent = originVertex->incomingParticle(i);
      if ( parent->absPdgId()<11 || parent->absPdgId()>16) continue;
      nlep++;
      float dR = (*it)->p4().DeltaR(parent->p4());
      if (dR < dRmin) dRmin = dR;
    }
    if ((nlep < 2) || (dRmin < 0.1)) continue;
    veto += 1000;
    break;
  }
  return veto;
}



bool  MCEventVetoHelper::trueBosonFromWorZplusJetsMCSample(
	 TLorentzVector& trueBoson, 
	 uint32_t mc_channel_number,
	 const xAOD::TruthParticleContainer* mcparticles)
{
  std::vector<TLorentzVector>true_leptons;

  for ( xAOD::TruthParticleContainer::const_iterator it = mcparticles->begin();
	it != mcparticles->end(); ++it ){
    int id = (*it)->absPdgId();


    //============================================================ 
    //In Sherpa the true boson 4momentum is computed 
    //from the 2 leptons with status==2
    //============================================================
    // FIXME comment says status==2, code status==3
    if ( id>=10 && id<=19 && (*it)->status()==3 )
    {
      TLorentzVector lep=TLorentzVector();
      true_leptons.push_back((*it)->p4());	  
    } 
    if(true_leptons.size()>=2) 
    {
      trueBoson=true_leptons[0]+true_leptons[1];	 
      return true;
    }
    
    //============================================================
    //In alpgen the true W or Z is the first boson with status=155
    //============================================================
    if ( (id==23 || id==24) && (*it)->status()==155  )
    {
      trueBoson=(*it)->p4();
      return true;
    }

  }
  return true;
}



bool MCEventVetoHelper::mc15SherpaWZjets_accept(
						uint32_t channel, 
						const xAOD::TruthParticleContainer* mcparticles)
{
  if ((channel >= 167740 && channel <= 167748) || (channel >= 167749 && channel <= 167760)) {// Sherpa W/Z+jets MassiveCB
    TLorentzVector vboson;
    bool isbosonfound = MCEventVetoHelper::trueBosonFromWorZplusJetsMCSample(vboson, channel, mcparticles);
    if (!isbosonfound) return true;
    // W+jets has a 40-70 bin in mc14  but not Z+jet
    if (channel >= 167740 && channel <= 167748 ) {
      if (vboson.Pt() > 40000.) return false;
    }
    else {
      if (vboson.Pt() > 70000.) return false;
    }

  } 
  return true;
}



bool MCEventVetoHelper::mc15accept(uint32_t mc_channel_number, const xAOD::TruthParticleContainer* mcparticles, const xAOD::MissingETContainer* metc)
{
  if ( MCEventVetoHelper::mc15SherpaWZjets_accept(mc_channel_number, mcparticles) ) return true;
  else return false;
}

