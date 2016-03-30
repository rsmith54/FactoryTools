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

#include "RJigsawTools/RJigsawCalculator_tls.h"
#include "EventLoop/StatusCode.h"
#include "xAODBase/ObjectType.h"

// this is needed to distribute the algorithm to the workers
ClassImp(RJigsawCalculator_tls)

RJigsawCalculator_tls :: RJigsawCalculator_tls() :
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
  InvRapidity_bkg  (nullptr),
  // ISR-identifying regions
  LAB_ISR          (nullptr),
  CM_ISR           (nullptr),
  S_ISR            (nullptr),
  ISR_ISR          (nullptr),
  V_ISR            (nullptr),
  I_ISR            (nullptr),
  INV_ISR          (nullptr),
  VIS_ISR          (nullptr),
  InvMass_ISR      (nullptr),
  SplitVis_ISR     (nullptr)
{}

RJigsawCalculator_tls :: ~RJigsawCalculator_tls() {
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
  delete  LAB_ISR      ;
  delete  CM_ISR       ;
  delete  S_ISR        ;
  delete  ISR_ISR      ;
  delete  V_ISR        ;
  delete  I_ISR        ;
  delete  INV_ISR      ;
  delete  VIS_ISR      ;
  delete  InvMass_ISR  ;
  delete  SplitVis_ISR ;
}

EL::StatusCode RJigsawCalculator_tls::doClearEvent() {

  if(! LAB    ->ClearEvent()){return EL::StatusCode::FAILURE;}
  if(! LAB_bkg->ClearEvent()){return EL::StatusCode::FAILURE;}
  if(! LAB_ISR->ClearEvent()){return EL::StatusCode::FAILURE;}

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_tls::doInitialize() {
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


  // ISR tree
  LAB_ISR = new LabRecoFrame("LAB_ISR","lab");
  CM_ISR = new DecayRecoFrame("CM_ISR","CM");
  S_ISR = new DecayRecoFrame("S_ISR","S");
  ISR_ISR = new VisibleRecoFrame("ISR_ISR","ISR");
  V_ISR = new VisibleRecoFrame("V_ISR","Vis");
  I_ISR = new InvisibleRecoFrame("I_ISR","Inv");

  LAB_ISR->SetChildFrame(*CM_ISR);
  CM_ISR->AddChildFrame(*ISR_ISR);
  CM_ISR->AddChildFrame(*S_ISR);
  S_ISR->AddChildFrame(*V_ISR);
  S_ISR->AddChildFrame(*I_ISR);

  LAB_ISR->InitializeTree();

  INV_ISR = new InvisibleGroup("INV_ISR","Invisible System");
  INV_ISR->AddFrame(*I_ISR);

  VIS_ISR = new CombinatoricGroup("VIS_ISR","Visible Objects");
  VIS_ISR->AddFrame(*ISR_ISR);
  VIS_ISR->SetNElementsForFrame(*ISR_ISR,1,false);
  VIS_ISR->AddFrame(*V_ISR);
  VIS_ISR->SetNElementsForFrame(*V_ISR,0,false);

  InvMass_ISR = new SetMassInvJigsaw("InvMass_ISR", "Invisible system mass Jigsaw");
  INV_ISR->AddJigsaw(*InvMass_ISR);

  SplitVis_ISR = new MinMassesCombJigsaw("SplitVis_ISR","Minimize M_{ISR} and M_{S} Jigsaw");
  VIS_ISR->AddJigsaw(*SplitVis_ISR);
  // "0" group (ISR)
  SplitVis_ISR->AddFrame(*ISR_ISR, 0);
  // "1" group (V + I)
  SplitVis_ISR->AddFrame(*V_ISR,1);
  SplitVis_ISR->AddFrame(*I_ISR,1);

  LAB_ISR->InitializeAnalysis();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_tls::doCalculate(std::map<std::string, double>& RJVars,
						   xAOD::IParticleContainer& particles,
						   xAOD::MissingET& met
						   ){
  if( particles.size() < 2 ){return EL::StatusCode::SUCCESS;}//todo figure out if this how we should handle this case

  using namespace RestFrames;
  using namespace std;

  TVector3 ETMiss(met.mpx() , met.mpy(), 0.) ;

  vector<TLorentzVector> Jets;//translate to the code from Chris
  vector<TLorentzVector> Leptons;

  for(auto particle : particles){
    TLorentzVector tmpPart;
    tmpPart.SetPtEtaPhiE(particle->p4().Pt(),
			 particle->p4().Eta(),
			 particle->p4().Phi(),
			 particle->p4().E()
			 );

    if ((particle->type() == xAOD::Type::Electron) ||
	 particle->type() == xAOD::Type::Muon    ){Leptons.push_back(tmpPart);}
    else if(particle->type() == xAOD::Type::Jet  ){Jets.push_back(tmpPart);   }
    else {
      std::cout << "You passed an unknown type of object to your particle container.  Returning EL::StatusCode::FAILURE";
      return EL::StatusCode::FAILURE;
    }
  }

  if(Leptons.size()>1){
    auto ptSort = [](TLorentzVector const & a , TLorentzVector const & b){return a.Pt() > b.Pt();};
    std::sort(Leptons.begin(), Leptons.end(), ptSort);
    assert(Leptons.at(0).Pt() > Leptons.at(1).Pt());
  }


  vector<RFKey> jetID;
  vector<RFKey> jetID_bkg;
  vector<RFKey> jetID_ISR;
  for(int i = 0; i < int(Jets.size()); i++){
    jetID.push_back(VIS->AddLabFrameFourVector(Jets[i]));

    Jets[i].SetPtEtaPhiM(Jets[i].Pt(),0.0,Jets[i].Phi(),Jets[i].M());
    jetID_bkg.push_back(VIS_bkg->AddLabFrameFourVector(Jets[i]));
    jetID_ISR.push_back(VIS_ISR->AddLabFrameFourVector(Jets[i]));
  }

  vector<RFKey> leptonID;
  vector<RFKey> leptonID_bkg;
  vector<RFKey> leptonID_ISR;
  for(int i = 0; i < int(Leptons.size()); i++){
    leptonID.push_back(VIS->AddLabFrameFourVector(Leptons[i]));
    Leptons[i].SetPtEtaPhiM(Leptons[i].Pt(),0.0,Leptons[i].Phi(),Leptons[i].M());
    leptonID_bkg.push_back(VIS_bkg->AddLabFrameFourVector(Leptons[i]));
    leptonID_ISR.push_back(VIS_ISR->AddLabFrameFourVector(Leptons[i]));
  }

  INV->SetLabFrameThreeVector(ETMiss);
  INV_bkg->SetLabFrameThreeVector(ETMiss);
  INV_ISR->SetLabFrameThreeVector(ETMiss);

  if(!LAB->AnalyzeEvent()) cout << "Something went wrong..." << endl;
  if(!LAB_bkg->AnalyzeEvent()) cout << "Something went wrong..." << endl;
  if(!LAB_ISR->AnalyzeEvent()) cout << "Something went wrong..." << endl;

  //  float const m_NJet = Jets.size();
  float const m_NJ1a = VIS->GetNElementsInFrame(*V1a);
  float const m_NJ1b = VIS->GetNElementsInFrame(*V1b);
  float const m_NJ2a = VIS->GetNElementsInFrame(*V2a);
  float const m_NJ2b = VIS->GetNElementsInFrame(*V2b);
  float const m_NJa = m_NJ1a+m_NJ2a;
  float const m_NJb = m_NJ1b+m_NJ2b;



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

  float m_temp_HT1CM     = -1.;
  float m_temp_PIoHT1CM  = -1.;
  float m_temp_cosS      = -1.;
  float m_temp_NVS       = -1.;
  float m_temp_NLepS     = 0.;
  float m_temp_RPT_HT1CM = -1.;
  float m_temp_MS        = -1.;
  float m_temp_dphiCMV   = -1.;

  float m_temp_dphiCML_0 = -1.;
  float m_temp_MTCM_L_0 = -1.;
  float m_temp_cosLINV_0 = -1.;
  float m_temp_dphiCML_1 = -1.;
  float m_temp_MTCM_L_1 = -1.;
  float m_temp_cosLINV_1 = -1.;

  m_temp_NVS = VIS_ISR->GetNElementsInFrame(*V_ISR);

  for(int j = 0; j < leptonID_ISR.size(); j++){
    if( VIS->GetFrame(leptonID_ISR.at(j) ) == *V_ISR ) m_temp_NLepS++;
  }

  if(m_temp_NVS < 1){
    m_temp_HT1CM = -1.;
    m_temp_PIoHT1CM = -1.;
    m_temp_cosS = -1.;
    m_temp_NVS = -1.;
    m_temp_NLepS     = -1.;
    m_temp_RPT_HT1CM = -1.;
    m_temp_MS = -1.;

    m_temp_dphiCML_0 = -1.;
    m_temp_MTCM_L_0 = -1.;
    m_temp_cosLINV_0 = -1.;
    m_temp_dphiCML_1 = -1.;
    m_temp_MTCM_L_1 = -1.;
    m_temp_cosLINV_1 = -1.;

  } else {

    TVector3 vP_ISR = ISR_ISR->GetFourVector(*CM_ISR).Vect();
    TVector3 vP_I   = I_ISR->GetFourVector(*CM_ISR).Vect();

    m_temp_HT1CM = vP_ISR.Mag();
    m_temp_MS = S_ISR->GetMass();

    if(m_temp_HT1CM > 0.)
      m_temp_PIoHT1CM = fabs(vP_I.Dot(vP_ISR.Unit())) / m_temp_HT1CM;
    else
      m_temp_PIoHT1CM = 0.;


    m_temp_cosS  = S_ISR->GetCosDecayAngle();
    m_temp_dphiCMV = CM_ISR->GetDeltaPhiBoostVisible();

    TLorentzVector PCM = CM_ISR->GetFourVector();
    m_temp_RPT_HT1CM = PCM.Pt() / ( PCM.Pt() + m_temp_HT1CM );


    TLorentzVector vL_CM, vCM_lab, vI_CM, vL_LI;
    TVector3 boostLI;

    vCM_lab = CM_ISR->GetFourVector();
    vI_CM = I_ISR->GetFourVector(*CM_ISR);
    if(Leptons.size()>0){
      vL_CM = CM_ISR->GetFourVector(Leptons.at(0) );
      m_temp_dphiCML_0 = acos( vL_CM.Vect().Unit().Dot(vCM_lab.Vect().Unit()) );
      m_temp_MTCM_L_0 = (vL_CM + vI_CM).M();
      boostLI = (vL_CM + vI_CM).BoostVector();
      vL_LI = vL_CM;
      vL_LI.Boost(-boostLI);
      m_temp_cosLINV_0 = boostLI.Unit().Dot(vL_LI.Vect().Unit());
    }
    if(Leptons.size()>1){
      vL_CM = CM_ISR->GetFourVector(Leptons.at(1) );
      m_temp_dphiCML_1 = acos( vL_CM.Vect().Unit().Dot(vCM_lab.Vect().Unit()) );
      m_temp_MTCM_L_1 = (vL_CM + vI_CM).M();
      boostLI = (vL_CM + vI_CM).BoostVector();
      vL_LI = vL_CM;
      vL_LI.Boost(-boostLI);
      m_temp_cosLINV_1 = boostLI.Unit().Dot(vL_LI.Vect().Unit());
    }

  }

  float const m_HT1CM     = m_temp_HT1CM     ;
  float const m_PIoHT1CM  = m_temp_PIoHT1CM  ;
  float const m_cosS      = m_temp_cosS      ;
  float const m_NVS       = m_temp_NVS       ;
  float const m_RPT_HT1CM = m_temp_RPT_HT1CM ;
  float const m_MS        = m_temp_MS;
  float const m_dphiCMV   = m_temp_dphiCMV;



  float const m_dphiCML_0 = m_temp_dphiCML_0;
  float const m_MTCM_L_0  = m_temp_MTCM_L_0 ;
  float const m_cosLINV_0 = m_temp_cosLINV_0;
  float const m_dphiCML_1 = m_temp_dphiCML_1;
  float const m_MTCM_L_1  = m_temp_MTCM_L_1 ;
  float const m_cosLINV_1 = m_temp_cosLINV_1;


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


  //Lepton tracking
  int temp_Nlep_a = 0;
  int temp_Nlep_b = 0;

  for(int j = 0; j < leptonID.size(); j++){
    const RestFrame & lepframe = VIS->GetFrame(leptonID.at(j) );
    if(V1a->IsSame(lepframe) || V2a->IsSame(lepframe)){temp_Nlep_a++;}
    if(V1b->IsSame(lepframe) || V2b->IsSame(lepframe)){temp_Nlep_b++;}
  }
                                                                                                                                                                     //float const m_Nlep_b = temp_Nlep_b;
  int m_sameHemi_lep = 0;
  if(temp_Nlep_a == 2||temp_Nlep_b ==2){m_sameHemi_lep = 1;}

  auto toGeV = [](float a){return a*.001;};

  RJVars[ "isSameHemi" ] = m_sameHemi_lep;
  RJVars[ "NLepS" ]     = m_temp_NLepS;

  RJVars[ "HT1CM"]     = toGeV(m_HT1CM);
  RJVars[ "PIoHT1CM"]  = m_PIoHT1CM;
  RJVars[ "cosS"]      = m_cosS;
  RJVars[ "NVS"]       = m_NVS;
  RJVars[ "RPT_HT1CM"] = m_RPT_HT1CM;
  RJVars[ "MS"]        = toGeV(m_MS);
  RJVars[ "dphiCMV"]   = m_dphiCMV;

  RJVars[ "dphiCML_0" ] = m_dphiCML_0 ;
  RJVars[ "MTCM_L_0"  ] = toGeV(m_MTCM_L_0)  ;
  RJVars[ "cosLINV_0" ] = m_cosLINV_0 ;
  RJVars[ "dphiCML_1" ] = m_dphiCML_1 ;
  RJVars[ "MTCM_L_1"  ] = toGeV(m_MTCM_L_1)  ;
  RJVars[ "cosLINV_1" ] = m_cosLINV_1 ;

  RJVars[ "ddphiP"] = m_ddphiP;
  RJVars[ "sdphiP"] = m_sdphiP;

  RJVars[ "pPP_Ia"] = m_pPP_Ia;
  RJVars[ "pPP_Ib"] = m_pPP_Ib;

  RJVars[ "pT_jet1a"] = toGeV(m_pT_jet1a);
  RJVars[ "pT_jet1b"] = toGeV(m_pT_jet1b);
  RJVars[ "pT_jet2a"] = toGeV(m_pT_jet2a);
  RJVars[ "pT_jet2b"] = toGeV(m_pT_jet2b);

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
  // RJVars["R_HT9PP_H9PP"] = m_R_HT9PP_H9PP;
  // RJVars["R_H2PP_H9PP"] = m_R_H2PP_H9PP;

  RJVars["RPZ_HT3PP"] = m_RPZ_HT3PP;
  RJVars["RPZ_HT5PP"] = m_RPZ_HT5PP;
  // RJVars["RPZ_HT9PP"] = m_RPZ_HT9PP;

  RJVars["RPT_HT3PP"] = m_RPT_HT3PP;
  RJVars["RPT_HT5PP"] = m_RPT_HT5PP;
  // RJVars["RPT_HT9PP"] = m_RPT_HT9PP;

  //  RJVars[ "PP_Mass"           ] = m_MPP;
  RJVars[ "PP_InvGamma"       ] = m_PP_VisShape;
  RJVars[ "PP_dPhiBetaR"      ] = m_dphiPPV;
  RJVars[ "PP_dPhiVis"        ] = PP->GetDeltaPhiVisible();
  RJVars[ "PP_CosTheta"       ] = m_cosPP;
  RJVars[ "PP_dPhiDecayAngle" ] = m_dphiVP ; // I think ...
  RJVars[ "PP_VisShape"       ] = m_PP_VisShape;
  RJVars[ "PP_MDeltaR"        ] = toGeV(m_MDR);

  // RJVars[ "P1_Mass"           ] = Pa->GetMass();
  RJVars[ "P1_CosTheta"       ] = m_cosP; //same as Pa->GetCosDecayAngle(*Ia)
  // RJVars[ "P2_Mass"           ] = Pb->GetMass();
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

  //RJVars[ "QCD_dPhiR"    ] = -100;
  //  RJVars[ "QCD_Rpt"      ] = m_RPT;
  RJVars[ "QCD_Rsib"    ]  = m_Rsib;
  RJVars[ "QCD_Delta1"   ] = m_deltaQCD;

  RJVars["H2PP"]      = toGeV(m_H2PP);
  RJVars["H3PP"]      = toGeV(m_H3PP);
  RJVars["H4PP"]      = toGeV(m_H4PP);
  RJVars["H6PP"]      = toGeV(m_H6PP);
  // RJVars["H10PP"]     = m_H10PP;

  // RJVars["HT10PP"]     = m_HT10PP;

  RJVars["H2Pa"]      = toGeV(m_H2Pa);
  RJVars["H2Pb"]      = toGeV(m_H2Pb);
  RJVars["H3Pa"]      = toGeV(m_H3Pa);
  RJVars["H3Pb"]      = toGeV(m_H3Pb);
  // RJVars["H4Pa"]      = m_H4Pa;
  // RJVars["H4Pb"]      = m_H4Pb;
  // RJVars["H5Pa"]      = m_H5Pa;
  // RJVars["H5Pb"]      = m_H5Pb;
  // RJVars["H2Ca"]      = m_H2Ca;
  // RJVars["H2Cb"]      = m_H2Cb;
  // RJVars["H3Ca"]      = m_H3Ca;
  // RJVars["H3Cb"]      = m_H3Cb;
  RJVars["HT4PP"]     = toGeV(m_HT4PP); //m_HT4PP;
  RJVars["HT6PP"]     = toGeV(m_HT6PP); //m_HT6PP;
  RJVars["sangle"]    = toGeV(m_sangle);
  RJVars["dangle"]    = toGeV(m_dangle);

  return EL::StatusCode::SUCCESS;
}

//function for debugging if need in the future :
// if( m_H2PP > 10e8) {
//   //todo clean this up, could be nice for debugging, probably move it to loop the map in here
//   std::cout << "WARNING : event with 10 TEV H2PP" << std::endl;
//   std::cout << __PRETTY_FUNCTION__ << " at line : " << __LINE__ << std::endl;

//   std::cout << "MET : " << ETMiss.Pt() << std::endl;
//   std::cout << "(vP_V1aPP + vP_V2aPP + vP_V1bPP + vP_V2bPP).P()" << (vP_V1aPP + vP_V2aPP + vP_V1bPP + vP_V2bPP).P() << std::endl;
//   std::cout << "vP_V1aPP.P() : " << vP_V1aPP.P() << std::endl;
//   std::cout << "vP_V2aPP.P() : " << vP_V2aPP.P() << std::endl;
//   std::cout << "vP_V1bPP.P() : " << vP_V1bPP.P() << std::endl;
//   std::cout << "vP_V2bPP.P() : " << vP_V2bPP.P() << std::endl;

//   std::cout << "vP_V1aPP.Pt() : " << vP_V1aPP.Pt() << std::endl;
//   std::cout << "vP_V2aPP.Pt() : " << vP_V2aPP.Pt() << std::endl;
//   std::cout << "vP_V1bPP.Pt() : " << vP_V1bPP.Pt() << std::endl;
//   std::cout << "vP_V2bPP.Pt() : " << vP_V2bPP.Pt() << std::endl;

//   std::cout << "(vP_IaPP+vP_IbPP).P() : " << (vP_IaPP+vP_IbPP).P() << std::endl;

//   int jetcounter = 0;
//   for (auto &jet : Jets) {
//     std::cout << "Jet " << jetcounter << " pT : " << jet.Perp() << std::endl;
//     std::cout << "Jet " << jetcounter << " Eta : " << jet.Eta() << std::endl;
//     std::cout << "Jet " << jetcounter << " Phi : " << jet.Phi() << std::endl;
//     std::cout << "Jet " << jetcounter << " M : " << jet.M() << std::endl;
//     jetcounter++;
//   }


//   int leptoncounter = 0;
//   for (auto &lepton : Leptons) {
//     std::cout << "Lepton " << leptoncounter << " pT : " << lepton.Perp() << std::endl;
//     std::cout << "Lepton " << leptoncounter << " Eta : " << lepton.Eta() << std::endl;
//     std::cout << "Lepton " << leptoncounter << " Phi : " << lepton.Phi() << std::endl;
//     std::cout << "Lepton " << leptoncounter << " M : " << lepton.M() << std::endl;
//     leptoncounter++;
//   }
// }
