#include "RestFrames/LabGenFrame.hh"
#include "RestFrames/ResonanceGenFrame.hh"
#include "RestFrames/ResonanceGenFrame.hh"
#include "RestFrames/ResonanceGenFrame.hh"
#include "RestFrames/VisibleGenFrame.hh"
#include "RestFrames/InvisibleGenFrame.hh"
#include "RestFrames/VisibleGenFrame.hh"
#include "RestFrames/InvisibleGenFrame.hh"

#include "RestFrames/LabRecoFrame.hh"
#include "RestFrames/DecayRecoFrame.hh"
#include "RestFrames/DecayRecoFrame.hh"
#include "RestFrames/DecayRecoFrame.hh"
#include "RestFrames/VisibleRecoFrame.hh"
#include "RestFrames/InvisibleRecoFrame.hh"
#include "RestFrames/VisibleRecoFrame.hh"
#include "RestFrames/InvisibleRecoFrame.hh"

#include "RestFrames/InvisibleGroup.hh"
#include "RestFrames/CombinatoricGroup.hh"
#include "RestFrames/MinMassesCombJigsaw.hh"
#include "RestFrames/SetMassInvJigsaw.hh"
#include "RestFrames/SetRapidityInvJigsaw.hh"
#include "RestFrames/ContraBoostInvJigsaw.hh"
#include "RestFrames/SelfAssemblingRecoFrame.hh"

#include "RJigsawTools/RJigsawCalculator_compressed.h"
#include "EventLoop/StatusCode.h"

// this is needed to distribute the algorithm to the workers
ClassImp(RJigsawCalculator_compressed)

RJigsawCalculator_compressed :: RJigsawCalculator_compressed() :
  LAB_bkg          (nullptr),
  S_bkg            (nullptr),
  V_bkg            (nullptr),
  I_bkg            (nullptr),
  INV_bkg          (nullptr),
  VIS_bkg          (nullptr),
  InvMass_bkg      (nullptr),
  InvRapidity_bkg  (nullptr)
{}

RJigsawCalculator_compressed :: ~RJigsawCalculator_compressed() {
  delete    LAB_bkg          ;
  delete    S_bkg            ;
  delete    V_bkg            ;
  delete    I_bkg            ;
  delete    INV_bkg          ;
  delete    VIS_bkg          ;
  delete    InvMass_bkg      ;
  delete    InvRapidity_bkg  ;
}

EL::StatusCode RJigsawCalculator_compressed::doClearEvent() {

  if(! LAB_bkg->ClearEvent()){return EL::StatusCode::FAILURE;}

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_compressed::doInitialize() {
  using namespace RestFrames;

  // RestFrames stuff

  LAB_bkg = new LabRecoFrame("LAB","lab");
  S_bkg   = new SelfAssemblingRecoFrame("CM","CM");
  V_bkg   = new VisibleRecoFrame("V_bkg","Vis");
  I_bkg   = new InvisibleRecoFrame("I_bkg","Inv");

  LAB_bkg->SetChildFrame(*S_bkg);
  S_bkg->AddChildFrame(*V_bkg);
  S_bkg->AddChildFrame(*I_bkg);

  LAB_bkg->InitializeTree();

  INV_bkg = new InvisibleGroup("INV_bkg","Invisible State Jigsaws");
  INV_bkg->AddFrame(*I_bkg);

  VIS_bkg = new CombinatoricGroup("VIS_bkg","Visible Object Jigsaws");
  VIS_bkg->AddFrame(*V_bkg);
  VIS_bkg->SetNElementsForFrame(*V_bkg,1,false);

  InvMass_bkg = new SetMassInvJigsaw("InvMass_bkg", "Invisible system mass Jigsaw");
  INV_bkg->AddJigsaw(*InvMass_bkg);

  InvRapidity_bkg = new SetRapidityInvJigsaw("InvRapidity_bkg", "Invisible system rapidity Jigsaw");
  INV_bkg->AddJigsaw(*InvRapidity_bkg);
  InvRapidity_bkg->AddVisibleFrames(LAB_bkg->GetListVisibleFrames());

  LAB_bkg->InitializeAnalysis();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_compressed::doCalculate(std::map<std::string, double>& RJVars,
               xAOD::IParticleContainer& particles,
               xAOD::MissingET& met
               ){
  if( particles.size() < 2 ){return EL::StatusCode::SUCCESS;}//todo figure out if this how we should handle this case

  using namespace RestFrames;
  using namespace std;

  TVector3 ETMiss(met.mpx() , met.mpy(), 0.) ;

  vector<TLorentzVector> Jets;//translate to the code from Chris


  for(auto particle : particles){
    TLorentzVector tmpPart;
    tmpPart.SetPtEtaPhiE(particle->p4().Pt(),
       particle->p4().Eta(),
       particle->p4().Phi(),
       particle->p4().E()
       );
    Jets.push_back(tmpPart);
  }

  double HT = 0.;
  vector<RFKey> jetID_bkg;
  for(int i = 0; i < int(Jets.size()); i++){
    Jets[i].SetPtEtaPhiM(Jets[i].Pt(),0.0,Jets[i].Phi(),Jets[i].M());
    jetID_bkg.push_back(VIS_bkg->AddLabFrameFourVector(Jets[i]));
    HT += Jets[i].Pt();
  }
  INV_bkg->SetLabFrameThreeVector(ETMiss);
  if(!LAB_bkg->AnalyzeEvent()) cout << "Something went wrong..." << endl;

  // QCD clean-up
  TLorentzVector Psib = I_bkg->GetSiblingFrame().GetFourVector(*LAB_bkg);
  TLorentzVector Pmet = I_bkg->GetFourVector(*LAB_bkg);

  float const m_temp_Rsib = max(0.,Psib.Vect().Dot(Pmet.Vect().Unit()));
  float const m_Rsib = m_temp_Rsib / (Pmet.Pt() + m_temp_Rsib);

  TVector3 boostQCD = (Pmet+Psib).BoostVector();
  Psib.Boost(-boostQCD);
  double cosQCD = -1.*Psib.Vect().Unit().Dot(boostQCD.Unit());
  cosQCD = (1.-cosQCD)/2.;
  float const m_deltaQCD = (cosQCD-m_Rsib)/(cosQCD+m_Rsib);

  // compressed
  float const m_Idepth = S_bkg->GetFrameDepth(*I_bkg);

  float m_temp_HT1CM     = 0.;
  float m_temp_PIoHT1CM  = 0.;
  float m_temp_cosS      = 0.;
  float m_temp_NVS       = 0.;
  float m_temp_RPT_HT1CM = 0.;
  float m_temp_MS = 0;

  if(m_Idepth < 2){
    m_temp_HT1CM = 0.;
    m_temp_PIoHT1CM = 0.;
    m_temp_cosS = 0.;
    m_temp_NVS = 0;
    m_temp_RPT_HT1CM = 0.;
  } else {
    const RestFrame& fS = S_bkg->GetFrameAtDepth(1, *I_bkg);
    m_temp_HT1CM = fS.GetTransverseMomentum(*S_bkg);
    m_temp_MS = fS.GetMass();

    m_temp_NVS = fS.GetNDescendants()-1;

    TLorentzVector vPV = fS.GetFourVector(*S_bkg) - I_bkg->GetFourVector(*S_bkg);
    TLorentzVector vPI = I_bkg->GetFourVector(*S_bkg);

    double PS = (vPV+vPI).P();
    double PIdot = max(0.,vPI.Vect().Dot((vPV+vPI).Vect().Unit()));
    m_temp_PIoHT1CM = PIdot / PS;

    m_temp_cosS  = -1.*fS.GetCosDecayAngle(*I_bkg);

    TLorentzVector PCM = S_bkg->GetFourVector();
    m_temp_RPT_HT1CM = PCM.Pt() / ( PCM.Pt() + m_temp_HT1CM );
  }

  float const m_HT1CM     = m_temp_HT1CM     ;
  float const m_PIoHT1CM  = m_temp_PIoHT1CM  ;
  float const m_cosS      = m_temp_cosS      ;
  float const m_NVS       = m_temp_NVS       ;
  float const m_RPT_HT1CM = m_temp_RPT_HT1CM ;
  float const m_MS        = m_temp_MS;

  auto toGeV = [](float a){return a*.001;};

  RJVars[ "HT1CM"]     = toGeV(m_HT1CM);
  RJVars[ "PIoHT1CM"]  = m_PIoHT1CM;
  RJVars[ "cosS"]      = m_cosS;
  RJVars[ "NVS"]       = m_NVS;
  RJVars[ "RPT_HT1CM"] = m_RPT_HT1CM;
  RJVars[ "MS"]        = toGeV(m_MS);


  return EL::StatusCode::SUCCESS;
}
