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

#include "RJigsawTools/RJigsawCalculator_zl.h"
#include "EventLoop/StatusCode.h"

// this is needed to distribute the algorithm to the workers
ClassImp(RJigsawCalculator_zl)

RJigsawCalculator_zl :: RJigsawCalculator_zl() :
LAB(nullptr),
  PP(nullptr),
  Pa(nullptr),
  Pb(nullptr),
  Ca(nullptr),
  Cb(nullptr),
  SAV1a(nullptr),
  SAV1b(nullptr),
  SAV2a(nullptr),
  SAV2b(nullptr),
  V1a(nullptr),
  V1b(nullptr),
  V2a(nullptr),
  V2b(nullptr),
  Ia(nullptr),
  Ib(nullptr),
  INV              (nullptr),
  VIS              (nullptr),
  InvMassJigsaw    (nullptr),
  InvRapidityJigsaw(nullptr),
  InvCBJigsaw      (nullptr),
  CombPPJigsaw     (nullptr),
  CombPaJigsaw     (nullptr),
  CombPbJigsaw     (nullptr),
  LAB_bkg          (nullptr),
  S_bkg            (nullptr),
  V_bkg            (nullptr),
  I_bkg            (nullptr),
  INV_bkg          (nullptr),
  VIS_bkg          (nullptr),
  InvMass_bkg      (nullptr),
  InvRapidity_bkg  (nullptr)
{}

RJigsawCalculator_zl :: ~RJigsawCalculator_zl() {
  delete  LAB;
  delete  PP;
  delete  Pa;
  delete  Pb;
  delete  Ca;
  delete  Cb;
  delete  SAV1a;
  delete  SAV1b;
  delete  SAV2a;
  delete  SAV2b;
  delete  V1a;
  delete  V1b;
  delete  V2a;
  delete  V2b;
  delete  Ia;
  delete  Ib;
  delete  INV              ;
  delete   VIS              ;
  delete    InvMassJigsaw    ;
  delete    InvRapidityJigsaw;
  delete    InvCBJigsaw      ;
  delete    CombPPJigsaw     ;
  delete    CombPaJigsaw     ;
  delete    CombPbJigsaw     ;
  delete    LAB_bkg          ;
  delete    S_bkg            ;
  delete    V_bkg            ;
  delete    I_bkg            ;
  delete    INV_bkg          ;
  delete    VIS_bkg          ;
  delete    InvMass_bkg      ;
  delete    InvRapidity_bkg  ;
}

EL::StatusCode RJigsawCalculator_zl::doClearEvent() {

  if(! LAB    ->ClearEvent()){return EL::StatusCode::FAILURE;}
  if(! LAB_bkg->ClearEvent()){return EL::StatusCode::FAILURE;}

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_zl::doInitialize() {
  using namespace RestFrames;

  // RestFrames stuff

  LAB = new LabRecoFrame("LAB","lab");
  PP = new DecayRecoFrame("PP","PP");
  Pa = new DecayRecoFrame("Pa","P_{a}");
  Pb = new DecayRecoFrame("Pb","P_{b}");
  Ca = new DecayRecoFrame("Ca","C_{a}");
  Cb = new DecayRecoFrame("Cb","C_{b}");
  SAV1a = new SelfAssemblingRecoFrame("SAV1a","SA_{V1a}");
  SAV1b = new SelfAssemblingRecoFrame("SAV1b","SA_{V1b}");
  SAV2a = new SelfAssemblingRecoFrame("SAV2a","SA_{V2a}");
  SAV2b = new SelfAssemblingRecoFrame("SAV2b","SA_{V2b}");
  V1a = new VisibleRecoFrame("V1a","V_{1a}");
  V1b = new VisibleRecoFrame("V1b","V_{1b}");
  V2a = new VisibleRecoFrame("V2a","V_{2a}");
  V2b = new VisibleRecoFrame("V2b","V_{2b}");
  Ia = new InvisibleRecoFrame("Ia","I_{a}");
  Ib = new InvisibleRecoFrame("Ib","I_{b}");

  LAB->SetChildFrame(*PP);
  PP->AddChildFrame(*Pa);
  PP->AddChildFrame(*Pb);
  Pa->AddChildFrame(*SAV1a);
  Pb->AddChildFrame(*SAV1b);
  Pa->AddChildFrame(*Ca);
  Pb->AddChildFrame(*Cb);

  SAV1a->AddChildFrame(*V1a);
  SAV1b->AddChildFrame(*V1b);

  Ca->AddChildFrame(*SAV2a);
  Cb->AddChildFrame(*SAV2b);
  Ca->AddChildFrame(*Ia);
  Cb->AddChildFrame(*Ib);

  SAV2a->AddChildFrame(*V2a);
  SAV2b->AddChildFrame(*V2b);

  LAB->InitializeTree();

  INV = new InvisibleGroup("INV","Invisible State Jigsaws");
  INV->AddFrame(*Ia);
  INV->AddFrame(*Ib);

  VIS = new CombinatoricGroup("VIS","Visible Object Jigsaws");
  VIS->AddFrame(*V1a);
  VIS->SetNElementsForFrame(*V1a,0,false);
  VIS->AddFrame(*V1b);
  VIS->SetNElementsForFrame(*V1b,0,false);
  VIS->AddFrame(*V2a);
  VIS->SetNElementsForFrame(*V2a,1,false);
  VIS->AddFrame(*V2b);
  VIS->SetNElementsForFrame(*V2b,1,false);

  InvMassJigsaw = new SetMassInvJigsaw("InvMassJigsaw", "Invisible system mass Jigsaw");
  INV->AddJigsaw(*InvMassJigsaw);

  InvRapidityJigsaw = new SetRapidityInvJigsaw("InvRapidityJigsaw", "Invisible system rapidity Jigsaw");
  INV->AddJigsaw(*InvRapidityJigsaw);
  InvRapidityJigsaw->AddVisibleFrames(LAB->GetListVisibleFrames());

  InvCBJigsaw = new ContraBoostInvJigsaw("InvCBJigsaw","Contraboost invariant Jigsaw");
  INV->AddJigsaw(*InvCBJigsaw);
  InvCBJigsaw->AddVisibleFrames(Pa->GetListVisibleFrames(), 0);
  InvCBJigsaw->AddVisibleFrames(Pb->GetListVisibleFrames(), 1);
  InvCBJigsaw->AddInvisibleFrame(*Ia, 0);
  InvCBJigsaw->AddInvisibleFrame(*Ib, 1);

  CombPPJigsaw = new MinMassesCombJigsaw("CombPPJigsaw","Minimize m _{V_{a,b}} Jigsaw");
  VIS->AddJigsaw(*CombPPJigsaw);
  CombPPJigsaw->AddFrame(*V1a,0);
  CombPPJigsaw->AddFrame(*V1b,1);
  CombPPJigsaw->AddFrame(*V2a,0);
  CombPPJigsaw->AddFrame(*V2b,1);

  CombPaJigsaw = new MinMassesCombJigsaw("C1HEM_JIGSAW","Minimize m _{C_{a}} Jigsaw");
  VIS->AddJigsaw(*CombPaJigsaw);
  CombPaJigsaw->AddFrame(*V1a,0);
  CombPaJigsaw->AddFrame(*V2a,1);

  CombPbJigsaw = new MinMassesCombJigsaw("C2HEM_JIGSAW","Minimize m _{C_{b}} Jigsaw");
  VIS->AddJigsaw(*CombPbJigsaw);
  CombPbJigsaw->AddFrame(*V1b,0);
  CombPbJigsaw->AddFrame(*V2b,1);

  LAB->InitializeAnalysis();

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

EL::StatusCode RJigsawCalculator_zl::doCalculate(std::map<std::string, double>& RJVars,
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

  vector<RFKey> jetID;
  for(int i = 0; i < int(Jets.size()); i++){
    jetID.push_back(VIS->AddLabFrameFourVector(Jets[i]));
  }
  INV->SetLabFrameThreeVector(ETMiss);
  if(!LAB->AnalyzeEvent()) cout << "Something went wrong..." << endl;

  //  float const m_NJet = Jets.size();
  float const m_NJ1a = VIS->GetNElementsInFrame(*V1a);
  float const m_NJ1b = VIS->GetNElementsInFrame(*V1b);
  float const m_NJ2a = VIS->GetNElementsInFrame(*V2a);
  float const m_NJ2b = VIS->GetNElementsInFrame(*V2b);
  float const m_NJa = m_NJ1a+m_NJ2a;
  float const m_NJb = m_NJ1b+m_NJ2b;

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


  // signal variables
  TLorentzVector vP_Va = Pa->GetVisibleFourVector(*Pa);
  TLorentzVector vP_Vb = Pb->GetVisibleFourVector(*Pb);

  TLorentzVector vP_V1aPP = V1a->GetFourVector(*PP);
  TLorentzVector vP_V2aPP = V2a->GetFourVector(*PP);
  TLorentzVector vP_V1bPP = V1b->GetFourVector(*PP);
  TLorentzVector vP_V2bPP = V2b->GetFourVector(*PP);
  TLorentzVector vP_IaPP  = Ia->GetFourVector(*PP);
  TLorentzVector vP_IbPP  = Ib->GetFourVector(*PP);

  TLorentzVector vP_V1aPa = V1a->GetFourVector(*Pa);
  TLorentzVector vP_V2aPa = V2a->GetFourVector(*Pa);
  TLorentzVector vP_IaPa  = Ia->GetFourVector(*Pa);
  TLorentzVector vP_V1bPb = V1b->GetFourVector(*Pb);
  TLorentzVector vP_V2bPb = V2b->GetFourVector(*Pb);
  TLorentzVector vP_IbPb  = Ib->GetFourVector(*Pb);

  float const m_H2PP = (vP_V1aPP + vP_V2aPP + vP_V1bPP + vP_V2bPP).P() + (vP_IaPP+vP_IbPP).P();
  float const m_H3PP = (vP_V1aPP + vP_V2aPP).P() + (vP_V1bPP + vP_V2bPP).P() + (vP_IaPP + vP_IbPP).P();
  float const m_H4PP = (vP_V1aPP + vP_V2aPP).P() + (vP_V1bPP + vP_V2bPP).P() + vP_IaPP.P() + vP_IbPP.P();
  float const m_H5PP = vP_V1aPP.P() + vP_V2aPP.P() + vP_V1bPP.P() + vP_V2bPP.P() + (vP_IaPP + vP_IbPP).P();
  float const m_H6PP = vP_V1aPP.P() + vP_V2aPP.P() + vP_V1bPP.P() + vP_V2bPP.P() + vP_IaPP.P() + vP_IbPP.P();

  float const m_H2Pa = (vP_V1aPa + vP_V2aPa).P() + vP_IaPa.P();
  float const m_H2Pb = (vP_V1bPb + vP_V2bPb).P() + vP_IbPb.P();
  float const m_H3Pa = vP_V1aPa.P() + vP_V2aPa.P() + vP_IaPa.P();
  float const m_H3Pb = vP_V1bPb.P() + vP_V2bPb.P() + vP_IbPb.P();

  float m_temp_H4Pa = 0.;
  float m_temp_H4Pb = 0.;
  float m_temp_H5Pa = 0.;
  float m_temp_H5Pb = 0.;

  float m_temp_H9PP  = 0.;
  float m_temp_H10PP = 0.;



  if(m_NJ1a > 1){
    m_temp_H4Pa += SAV1a->GetChildFrame(0).GetMomentum(*Pa);
    m_temp_H4Pa += SAV1a->GetChildFrame(1).GetMomentum(*Pa);
    m_temp_H5Pa += m_temp_H4Pa;
    m_temp_H9PP += SAV1a->GetChildFrame(0).GetMomentum(*PP);
    m_temp_H9PP += SAV1a->GetChildFrame(1).GetMomentum(*PP);
  } else {
    m_temp_H4Pa += vP_V1aPa.P();
    m_temp_H5Pa += vP_V1aPa.P();
    m_temp_H9PP += vP_V1aPP.P();
  }
  if(m_NJ1b > 1){
    m_temp_H4Pb += SAV1b->GetChildFrame(0).GetMomentum(*Pb);
    m_temp_H4Pb += SAV1b->GetChildFrame(1).GetMomentum(*Pb);
    m_temp_H5Pb += m_temp_H4Pb;
    m_temp_H9PP += SAV1b->GetChildFrame(0).GetMomentum(*PP);
    m_temp_H9PP += SAV1b->GetChildFrame(1).GetMomentum(*PP);
  } else {
    m_temp_H4Pb += vP_V1bPb.P();
    m_temp_H5Pb += vP_V1bPb.P();
    m_temp_H9PP += vP_V1bPP.P();
  }
  m_temp_H4Pa += vP_V2aPa.P();
  m_temp_H4Pb += vP_V2bPb.P();

  if(m_NJ2a > 1){
    m_temp_H5Pa += SAV2a->GetChildFrame(0).GetMomentum(*Pa);
    m_temp_H5Pa += SAV2a->GetChildFrame(1).GetMomentum(*Pa);
    m_temp_H9PP += SAV2a->GetChildFrame(0).GetMomentum(*PP);
    m_temp_H9PP += SAV2a->GetChildFrame(1).GetMomentum(*PP);
  } else {
    m_temp_H5Pa += vP_V2aPa.P();
    m_temp_H9PP += vP_V2aPP.P();
  }
  if(m_NJ2b > 1){
    m_temp_H5Pb += SAV2b->GetChildFrame(0).GetMomentum(*Pb);
    m_temp_H5Pb += SAV2b->GetChildFrame(1).GetMomentum(*Pb);
    m_temp_H9PP += SAV2b->GetChildFrame(0).GetMomentum(*PP);
    m_temp_H9PP += SAV2b->GetChildFrame(1).GetMomentum(*PP);
  } else {
    m_temp_H5Pb += vP_V2bPb.P();
    m_temp_H9PP += vP_V2bPP.P();
  }
  m_temp_H4Pa += vP_IaPa.P();
  m_temp_H5Pa += vP_IaPa.P();
  m_temp_H4Pb += vP_IbPb.P();
  m_temp_H5Pb += vP_IbPb.P();

  m_temp_H10PP = m_temp_H9PP;

  m_temp_H9PP  += (vP_IaPP + vP_IbPP).P();
  m_temp_H10PP += vP_IaPP.P() + vP_IbPP.P();

  float const  m_H4Pa  = m_temp_H4Pa ;
  float const  m_H4Pb  = m_temp_H4Pb ;
  float const  m_H5Pa  = m_temp_H5Pa ;
  float const  m_H5Pb  = m_temp_H5Pb ;
  float const  m_H9PP  = m_temp_H9PP ;
  float const  m_H10PP = m_temp_H10PP;

  TLorentzVector vP_IaCa  = Ia->GetFourVector(*Ca);
  TLorentzVector vP_IbCb  = Ib->GetFourVector(*Cb);

  float m_temp_H2Ca = 2.*vP_IaCa.P();
  float m_temp_H2Cb = 2.*vP_IbCb.P();

  float m_temp_H3Ca = 0;
  float m_temp_H3Cb = 0;

  if(m_NJ2a > 1)
    m_temp_H3Ca = vP_IaCa.P()+
      SAV2a->GetChildFrame(0).GetMomentum(*Ca)+
      SAV2a->GetChildFrame(1).GetMomentum(*Ca);
  else
    m_temp_H3Ca = m_temp_H2Ca;

  if(m_NJ2b > 1)
    m_temp_H3Cb = vP_IbCb.P()+
      SAV2b->GetChildFrame(0).GetMomentum(*Cb)+
      SAV2b->GetChildFrame(1).GetMomentum(*Cb);
  else
    m_temp_H3Cb = m_temp_H2Cb;

  float const m_H3Ca = m_temp_H3Ca;
  float const m_H3Cb = m_temp_H3Cb;

  float const m_H2Ca = m_temp_H2Ca;
  float const m_H2Cb = m_temp_H2Cb;



  TVector3 vP_PP = PP->GetFourVector(*LAB).Vect();

  float const m_PP_VisShape = PP->GetVisibleShape();
  float const m_MDR = m_PP_VisShape*PP->GetMass();


  float const m_cosPP = PP->GetCosDecayAngle();


  float const m_dphiPPV = PP->GetDeltaPhiBoostVisible();
  float const m_cosP = Pa->GetCosDecayAngle(*Ia);

  float const m_temp_dphiVP = PP->GetDeltaPhiDecayVisible();
  float const m_dphiVP = (m_temp_dphiVP-acos(-1.)/2.)/(acos(-1.)/2.);

  float const m_sangle = fabs(m_dphiVP + 2.*m_cosP)/3.;
  float const m_dangle = (2.*m_dphiVP - m_cosP)/3.;

  // gluino hemishpere variables
  float const m_dphiPa = Pa->GetDeltaPhiDecayPlanes(*Ca);
  float const m_dphiPb = Pb->GetDeltaPhiDecayPlanes(*Cb);

  double dphiA = m_dphiPa;
  double dphiB = m_dphiPb;
  if(dphiA > acos(-1.))
    dphiA = 2.*acos(-1.) - dphiA;
  if(dphiB > acos(-1.))
    dphiB = 2.*acos(-1.) - dphiB;
  float const  m_ddphiP = (dphiA-dphiB)/acos(-1.);
  float const  m_sdphiP = fabs(dphiA+dphiB)/2./acos(-1.);

  // inside gluino hemisphere variables
  float const m_dphiPV1a = Pa->GetDeltaPhiDecayPlanes(*SAV1a);
  float const m_dphiPV1b = Pb->GetDeltaPhiDecayPlanes(*SAV1b);
  float const m_cosV1a = SAV1a->GetCosDecayAngle();
  float const m_cosV1b = SAV1b->GetCosDecayAngle();
  float const m_dphiCV2a = Ca->GetDeltaPhiDecayPlanes(*SAV2a);
  float const m_dphiCV2b = Cb->GetDeltaPhiDecayPlanes(*SAV2b);
  float const m_cosV2a = SAV2a->GetCosDecayAngle();
  float const m_cosV2b = SAV2b->GetCosDecayAngle();

  //float const m_MET = ETMiss.Pt();
  //  float const m_Meff = NTVars_meffInc;
  //  float const m_Aplan = NTExtraVars_Ap;
  //  float const m_dphi = NTVars_dPhi;
  //  float const m_dphiR = NTVars_dPhiR;

  // float const m_pT_jet1 = Jets[0].Pt();
  // float const m_pT_jet2 = Jets[1].Pt();
  // if(m_NJet >= 3)
  //   m_pT_jet3 = Jets[2].Pt();
  // else
  //   m_pT_jet3 = 0.;
  // if(m_NJet >= 3)
  //   m_pT_jet4 = Jets[3].Pt();
  // else
  //   m_pT_jet4 = 0.;
  // if(m_NJet >= 3)
  //   m_pT_jet5 = Jets[4].Pt();
  // else
  //   m_pT_jet5 = 0.;
  // if(m_NJet >= 3)
  //   m_pT_jet6 = Jets[5].Pt();
  // else
  //   m_pT_jet6 = 0.;

  float const m_pTPP_Va  = PP->GetTransverseMomentum(V1a->GetFourVector()+V2a->GetFourVector());
  float const m_pTPP_V1a = V1a->GetTransverseMomentum(*PP);
  float const m_pTPP_V2a = V2a->GetTransverseMomentum(*PP);
  float const m_pTPP_Vb  = PP->GetTransverseMomentum(V1b->GetFourVector()+V2b->GetFourVector());
  float const m_pTPP_V1b = V1b->GetTransverseMomentum(*PP);
  float const m_pTPP_V2b = V2b->GetTransverseMomentum(*PP);
  float const m_pTPP_Ia = Ia->GetTransverseMomentum(*PP);
  float const m_pTPP_Ib = Ib->GetTransverseMomentum(*PP);

  float const m_pPP_Va  = (V1a->GetFourVector(*PP)+V2a->GetFourVector(*PP)).P();
  float const m_pPP_V1a = V1a->GetMomentum(*PP);
  float const m_pPP_V2a = V2a->GetMomentum(*PP);
  float const m_pPP_Vb  = (V1b->GetFourVector(*PP)+V2b->GetFourVector(*PP)).P();
  float const m_pPP_V1b = V1b->GetMomentum(*PP);
  float const m_pPP_V2b = V2b->GetMomentum(*PP);
  float const m_pPP_Ia = Ia->GetMomentum(*PP);
  float const m_pPP_Ib = Ib->GetMomentum(*PP);

  float  m_pT_temp_jet1a = 0.;
  float  m_pT_temp_jet2a = 0.;
  float  m_pT_temp_jet1b = 0.;
  float  m_pT_temp_jet2b = 0.;

  int N = jetID.size();
  for(int j = 0; j < N; j++){
    RestFrame const& frame = VIS->GetFrame(jetID[j]);
    double pT = VIS->GetLabFrameFourVector(jetID[j]).Pt();

    if(V1a->IsSame(frame) || V2a->IsSame(frame)){
      if(pT > m_pT_temp_jet1a){
	m_pT_temp_jet2a = m_pT_temp_jet1a;
	m_pT_temp_jet1a = pT;
      } else {
	if(pT > m_pT_temp_jet2a){
	  m_pT_temp_jet2a = pT;
	}
      }
    }
    if(V1b->IsSame(frame) || V2b->IsSame(frame)){
      if(pT > m_pT_temp_jet1b){
	m_pT_temp_jet2b = m_pT_temp_jet1b;
	m_pT_temp_jet1b = pT;
      } else {
	if(pT > m_pT_temp_jet2b){
	  m_pT_temp_jet2b = pT;
	}
      }
    }
  }

  float const m_pT_jet1a = m_pT_temp_jet1a;
  float const m_pT_jet2a = m_pT_temp_jet2a;
  float const m_pT_jet1b = m_pT_temp_jet1b;
  float const m_pT_jet2b = m_pT_temp_jet2b;


  float m_pTPP_temp_jet1a = 0.;
  float m_pTPP_temp_jet2a = 0.;
  float m_pTPP_temp_jet3a = 0.;
  float m_pTPP_temp_jet1b = 0.;
  float m_pTPP_temp_jet2b = 0.;
  float m_pTPP_temp_jet3b = 0.;

  float m_pPP_temp_jet1a = 0.;
  float m_pPP_temp_jet2a = 0.;
  float m_pPP_temp_jet3a = 0.;
  float m_pPP_temp_jet1b = 0.;
  float m_pPP_temp_jet2b = 0.;
  float m_pPP_temp_jet3b = 0.;

  float m_pTPP_temp_jet1 = 0.;
  float m_pTPP_temp_jet2 = 0.;

  for(int j = 0; j < N; j++){
    RestFrame const& frame = VIS->GetFrame(jetID[j]);
    double pT = PP->GetTransverseMomentum(VIS->GetLabFrameFourVector(jetID[j]));
    double p  = PP->GetFourVector(VIS->GetLabFrameFourVector(jetID[j])).P();

    if(V1a->IsSame(frame) || V2a->IsSame(frame)){
      if(pT > m_pTPP_temp_jet1a){
	m_pTPP_temp_jet3a = m_pTPP_temp_jet2a;
	m_pPP_temp_jet3a  = m_pPP_temp_jet2a;
	m_pTPP_temp_jet2a = m_pTPP_temp_jet1a;
	m_pPP_temp_jet2a  = m_pPP_temp_jet1a;
	m_pTPP_temp_jet1a = pT;
	m_pPP_temp_jet1a  = p;
      } else {
	if(pT > m_pTPP_temp_jet2a){
	  m_pTPP_temp_jet3a = m_pTPP_temp_jet2a;
	  m_pPP_temp_jet3a  = m_pPP_temp_jet2a;
	  m_pTPP_temp_jet2a = pT;
	  m_pPP_temp_jet2a  = p;
	} else{
	  if(pT > m_pTPP_temp_jet3a){
	    m_pTPP_temp_jet3a = pT;
	    m_pPP_temp_jet3a  = p;
	  }
	}
      }
    }
    if(V1b->IsSame(frame) || V2b->IsSame(frame)){
      if(pT > m_pTPP_temp_jet1b){
	m_pTPP_temp_jet3b = m_pTPP_temp_jet2b;
	m_pPP_temp_jet3b  = m_pPP_temp_jet2b;
	m_pTPP_temp_jet2b = m_pTPP_temp_jet1b;
	m_pPP_temp_jet2b  = m_pPP_temp_jet1b;
	m_pTPP_temp_jet1b = pT;
	m_pPP_temp_jet1b  = p;
      } else {
	if(pT > m_pTPP_temp_jet2b){
	  m_pTPP_temp_jet3b = m_pTPP_temp_jet2b;
	  m_pPP_temp_jet3b  = m_pPP_temp_jet2b;
	  m_pTPP_temp_jet2b = pT;
	  m_pPP_temp_jet2b  = p;
	} else{
	  if(pT > m_pTPP_temp_jet3b){
	    m_pTPP_temp_jet3b = pT;
	    m_pPP_temp_jet3b  = p;
	  }
	}
      }
    }
  }

  if(m_pTPP_temp_jet1a > m_pTPP_temp_jet1b){
    m_pTPP_temp_jet1 = m_pTPP_temp_jet1a;
    m_pTPP_temp_jet2 = max(m_pTPP_temp_jet1b,m_pTPP_temp_jet2a);
  } else {
    m_pTPP_temp_jet1 = m_pTPP_temp_jet1b;
    m_pTPP_temp_jet2 = max(m_pTPP_temp_jet1a,m_pTPP_temp_jet2b);
  }

  float const m_pTPP_jet1a = m_pTPP_temp_jet1a;
  float const m_pTPP_jet2a = m_pTPP_temp_jet2a;
  float const m_pTPP_jet3a = m_pTPP_temp_jet3a;
  float const m_pTPP_jet1b = m_pTPP_temp_jet1b;
  float const m_pTPP_jet2b = m_pTPP_temp_jet2b;
  float const m_pTPP_jet3b = m_pTPP_temp_jet3b;

  float const m_pPP_jet1a = m_pPP_temp_jet1a;
  float const m_pPP_jet2a = m_pPP_temp_jet2a;
  float const m_pPP_jet3a = m_pPP_temp_jet3a;
  float const m_pPP_jet1b = m_pPP_temp_jet1b;
  float const m_pPP_jet2b = m_pPP_temp_jet2b;
  float const m_pPP_jet3b = m_pPP_temp_jet3b;

  float const m_pTPP_jet1 = m_pTPP_temp_jet1;
  float const m_pTPP_jet2 = m_pTPP_temp_jet2;


  float const m_HT3PP = m_pTPP_Va + m_pTPP_Vb +
            m_H2PP/2.;
  float const m_HT4PP = m_pTPP_Va + m_pTPP_Vb +
            m_pTPP_Ia + m_pTPP_Ib;
  float const m_HT5PP = m_pTPP_V1a + m_pTPP_V1b +
            m_pTPP_V2a + m_pTPP_V2b +
            m_H2PP/2.;
  float const m_HT6PP = m_pTPP_V1a + m_pTPP_V1b +
            m_pTPP_V2a + m_pTPP_V2b +
            m_pTPP_Ia + m_pTPP_Ib;

  float m_temp_HT9PP = 0;
  if(m_NJ1a > 1){
    m_temp_HT9PP += SAV1a->GetChildFrame(0).GetTransverseMomentum(*PP);
    m_temp_HT9PP += SAV1a->GetChildFrame(1).GetTransverseMomentum(*PP);
  } else {
    m_temp_HT9PP += m_pTPP_V1a;
  }
  if(m_NJ1b > 1){
    m_temp_HT9PP += SAV1b->GetChildFrame(0).GetTransverseMomentum(*PP);
    m_temp_HT9PP += SAV1b->GetChildFrame(1).GetTransverseMomentum(*PP);
  } else {
    m_temp_HT9PP += m_pTPP_V1b;
  }
  if(m_NJ2a > 1){
    m_temp_HT9PP += SAV2a->GetChildFrame(0).GetTransverseMomentum(*PP);
    m_temp_HT9PP += SAV2a->GetChildFrame(1).GetTransverseMomentum(*PP);
  } else {
    m_temp_HT9PP += m_pTPP_V2a;
  }
  if(m_NJ2b > 1){
    m_temp_HT9PP += SAV2b->GetChildFrame(0).GetTransverseMomentum(*PP);
    m_temp_HT9PP += SAV2b->GetChildFrame(1).GetTransverseMomentum(*PP);
  } else {
    m_temp_HT9PP += m_pTPP_V2b;
  }

  float const m_temp_HT10PP = m_temp_HT9PP;

  float const m_HT9PP  = m_temp_HT9PP + m_H2PP/2.;
  float const m_HT10PP = m_temp_HT10PP + m_pTPP_Ia + m_pTPP_Ib;

  /// squark ratios
  float const m_R_H2PP_H3PP = m_H2PP/m_H3PP;
  float const m_R_pTj2_HT3PP = m_pTPP_jet2 / m_HT3PP;

  /// gluino ratios
  float const m_R_HT5PP_H5PP = m_HT5PP/m_H5PP;
  float const m_R_H2PP_H5PP = m_H2PP/m_H5PP;
  float const m_minR_pTj2i_HT3PPi = min(m_pTPP_jet2a/(m_pTPP_V1a+m_pTPP_V2a+m_pTPP_Ia),
			    m_pTPP_jet2b/(m_pTPP_V1b+m_pTPP_V2b+m_pTPP_Ib));
  float const m_maxR_H1PPi_H2PPi = max(m_pPP_Va/(m_pPP_V1a+m_pPP_V2a),m_pPP_Vb/(m_pPP_V1b+m_pPP_V2b));

   /// GG onestep ratios
  float const m_R_HT9PP_H9PP = m_HT5PP/m_H9PP;
  float const m_R_H2PP_H9PP = m_H2PP/m_H9PP;

  //  float const m_dH2o3P = m_H2Pa/m_H3Pa - m_H2Pb/m_H3Pb;

  float const m_pTCM = vP_PP.Pt();
  float const m_pZCM = fabs(vP_PP.Pz());

  float const m_RPZ_HT3PP = m_pZCM / (m_pZCM + m_HT3PP);
  float const m_RPZ_HT5PP = m_pZCM / (m_pZCM + m_HT5PP);
  float const m_RPZ_HT9PP = m_pZCM / (m_pZCM + m_HT9PP);
  float const m_RPT_HT3PP = m_pTCM / (m_pTCM + m_HT3PP);
  float const m_RPT_HT5PP = m_pTCM / (m_pTCM + m_HT5PP);
  float const m_RPT_HT9PP = m_pTCM / (m_pTCM + m_HT9PP);

  auto toGeV = [](float a){return a*.001;};

  RJVars[ "HT1CM"]     = toGeV(m_HT1CM);
  RJVars[ "PIoHT1CM"]  = m_PIoHT1CM;
  RJVars[ "cosS"]      = m_cosS;
  RJVars[ "NVS"]       = m_NVS;
  RJVars[ "RPT_HT1CM"] = m_RPT_HT1CM;
  RJVars[ "MS"]        = toGeV(m_MS);

  RJVars[ "ddphiP"]      = m_ddphiP;
  RJVars[ "sdphiP"] = m_sdphiP;

  RJVars[ "pPP_Ia"] = m_pPP_Ia;
  RJVars[ "pPP_Ib"] = m_pPP_Ib;

  RJVars[ "pT_jet1a"]  = toGeV(m_pT_jet1a);
  RJVars[ "pT_jet1b"]  = toGeV(m_pT_jet1b);
  RJVars[ "pT_jet2a"]  = toGeV(m_pT_jet2a);
  RJVars[ "pT_jet2b"]  = toGeV(m_pT_jet2b);
  RJVars[ "pTPP_jet1"] = toGeV(m_pTPP_jet1);
  RJVars[ "pTPP_jet2"] = toGeV(m_pTPP_jet2);

  RJVars[ "pTPP_jet1a"] = toGeV(m_pTPP_jet1a);
  RJVars[ "pTPP_jet1b"] = toGeV(m_pTPP_jet1b);
  RJVars[ "pTPP_jet2a"] = toGeV(m_pTPP_jet2a);
  RJVars[ "pTPP_jet2b"] = toGeV(m_pTPP_jet2b);
  RJVars[ "pTPP_jet3a"] = toGeV(m_pTPP_jet3a);
  RJVars[ "pTPP_jet3b"] = toGeV(m_pTPP_jet3b);

  RJVars[ "pPP_jet1a"] = toGeV(m_pPP_jet1a);
  RJVars[ "pPP_jet1b"] = toGeV(m_pPP_jet1b);
  RJVars[ "pPP_jet2a"] = toGeV(m_pPP_jet2a);
  RJVars[ "pPP_jet2b"] = toGeV(m_pPP_jet2b);
  RJVars[ "pPP_jet3a"] = toGeV(m_pPP_jet3a);
  RJVars[ "pPP_jet3b"] = toGeV(m_pPP_jet3b);

  RJVars["R_H2PP_H3PP"]  = m_R_H2PP_H3PP;
  RJVars["R_pTj2_HT3PP"] = m_R_pTj2_HT3PP;
  RJVars["R_HT5PP_H5PP"] = m_R_HT5PP_H5PP;
  RJVars["R_H2PP_H5PP"] = m_R_H2PP_H5PP;
  RJVars["minR_pTj2i_HT3PPi"] = m_minR_pTj2i_HT3PPi;
  RJVars["maxR_H1PPi_H2PPi"] = m_maxR_H1PPi_H2PPi;
  RJVars["R_HT9PP_H9PP"] = m_R_HT9PP_H9PP;
  RJVars["R_H2PP_H9PP"] = m_R_H2PP_H9PP;

  RJVars["RPZ_HT3PP"] = m_RPZ_HT3PP;
  RJVars["RPZ_HT5PP"] = m_RPZ_HT5PP;
  RJVars["RPZ_HT9PP"] = m_RPZ_HT9PP;

  RJVars["RPT_HT3PP"] = m_RPT_HT3PP;
  RJVars["RPT_HT5PP"] = m_RPT_HT5PP;
  RJVars["RPT_HT9PP"] = m_RPT_HT9PP;

  //  RJVars[ "PP_Mass"           ] = m_MPP;
  RJVars[ "PP_InvGamma"       ] = m_PP_VisShape;
  RJVars[ "PP_dPhiBetaR"      ] = m_dphiPPV;
  RJVars[ "PP_dPhiVis"        ] = PP->GetDeltaPhiVisible();
  RJVars[ "PP_CosTheta"       ] = m_cosPP;
  RJVars[ "PP_dPhiDecayAngle" ] = m_dphiVP ; // I think ...
  RJVars[ "PP_VisShape"       ] = m_PP_VisShape;
  RJVars[ "PP_MDeltaR"        ] = toGeV(m_MDR);

  RJVars[ "P1_Mass"           ] = toGeV(Pa->GetMass());
  RJVars[ "P1_CosTheta"       ] = m_cosP; //same as Pa->GetCosDecayAngle(*Ia)
  RJVars[ "P2_Mass"           ] = toGeV(Pb->GetMass());
  RJVars[ "P2_CosTheta"       ] = Pb->GetCosDecayAngle(*Ib); //I think ...-100;
  RJVars[ "I1_Depth"          ] = Pa->GetFrameDepth   (*Ia);
  RJVars[ "I2_Depth"          ] = Pb->GetFrameDepth   (*Ib);

  RJVars["dphiPV1a"  ] = m_dphiPV1a;
  RJVars["cosV1a"    ] = m_cosV1a;
  RJVars["dphiCV2a"  ] = m_dphiCV2a;
  RJVars["cosV2a"    ] = m_cosV2a;
  RJVars["dphiPV1b" ]  = m_dphiPV1b;
  RJVars["cosV1b"   ]  = m_cosV1b;
  RJVars["dphiCV2b" ]  = m_dphiCV2b;
  RJVars["cosV2b"]	= m_cosV2b;

  RJVars["NJa"] = m_NJa;
  RJVars["NJb"] = m_NJb;
  //  RJVars[ "MP"          ] = m_MP;
  //  RJVars[ "DeltaBetaGG" ] = DeltaBetaGG;
  RJVars[ "dphiVG"      ] = PP->GetDeltaPhiDecayVisible();

  RJVars[ "QCD_dPhiR"    ] = -100;
  //  RJVars[ "QCD_Rpt"      ] = m_RPT;
  RJVars[ "QCD_Rsib"    ]  = m_Rsib;
  RJVars[ "QCD_Delta1"   ] = m_deltaQCD;

  RJVars["H2PP"]      = toGeV(m_H2PP) ;
  RJVars["H3PP"]      = toGeV(m_H3PP);
  RJVars["H4PP"]      = toGeV(m_H4PP);
  RJVars["H6PP"]      = toGeV(m_H6PP);
  RJVars["H10PP"]     = toGeV(m_H10PP);
  RJVars["HT10PP"]    = toGeV(m_HT10PP);

  RJVars["H2Pa"]      = toGeV(m_H2Pa);
  RJVars["H2Pb"]      = toGeV(m_H2Pb);
  RJVars["H3Pa"]      = toGeV(m_H3Pa);
  RJVars["H3Pb"]      = toGeV(m_H3Pb);
  RJVars["H4Pa"]      = toGeV(m_H4Pa);
  RJVars["H4Pb"]      = toGeV(m_H4Pb);
  RJVars["H5Pa"]      = toGeV(m_H5Pa);
  RJVars["H5Pb"]      = toGeV(m_H5Pb);
  RJVars["H2Ca"]      = toGeV(m_H2Ca);
  RJVars["H2Cb"]      = toGeV(m_H2Cb);
  RJVars["H3Ca"]      = toGeV(m_H3Ca);
  RJVars["H3Cb"]      = toGeV(m_H3Cb);
  RJVars["HT4PP"]     = toGeV(m_HT4PP); //m_HT4PP;
  RJVars["HT6PP"]     = toGeV(m_HT6PP); //m_HT6PP;
  //  RJVars["minH3P"]  toGeV(  = m_minH3P; //m_minH3P;
  RJVars["sangle"]    = m_sangle;
  RJVars["dangle"]    = m_dangle;

  return EL::StatusCode::SUCCESS;
}
