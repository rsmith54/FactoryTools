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
#include "RestFrames/SetMassInvJigsaw.hh"
#include "RestFrames/SetRapidityInvJigsaw.hh"
#include "RestFrames/ContraBoostInvJigsaw.hh"

#include "RJigsawTools/RJigsawCalculator_lvlv.h"
#include "EventLoop/StatusCode.h"

// this is needed to distribute the algorithm to the workers
ClassImp(RJigsawCalculator_lvlv)

RJigsawCalculator_lvlv :: RJigsawCalculator_lvlv() :
  LAB_R(nullptr),
  H_R(nullptr),
  Wa_R(nullptr),
  Wb_R(nullptr),
  La_R(nullptr),
  Na_R(nullptr),
  Lb_R(nullptr),
  Nb_R(nullptr),
  INV_R(nullptr),
  MinMassJigsaw_R(nullptr),
  RapidityJigsaw_R(nullptr),
  ContraBoostJigsaw_R(nullptr)
{}

RJigsawCalculator_lvlv :: ~RJigsawCalculator_lvlv() {
  delete LAB_R;
  delete H_R;
  delete Wa_R;
  delete Wb_R;
  delete La_R;
  delete Na_R;
  delete Lb_R;
  delete Nb_R;
  delete INV_R;
  delete MinMassJigsaw_R;
  delete RapidityJigsaw_R;
  delete ContraBoostJigsaw_R;
}

EL::StatusCode RJigsawCalculator_lvlv::doClearEvent() {
  if(! LAB_R->ClearEvent()){return EL::StatusCode::FAILURE;}

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_lvlv::doInitialize() {
  using namespace RestFrames;

  // Set up reco analysis tree
  LAB_R = new   LabRecoFrame     ("LAB_R","LAB");
  H_R   = new     DecayRecoFrame   ("H_R","H");
  Wa_R  = new	DecayRecoFrame   ("Wa_R","W_{a}");
  Wb_R  = new	DecayRecoFrame   ("Wb_R","W_{b}");
  La_R  = new    VisibleRecoFrame ("La_R","#it{l}_{a}");
  Na_R  = new  InvisibleRecoFrame ("Na_R","#nu_{a}");
  Lb_R  = new    VisibleRecoFrame ("Lb_R","#it{l}_{b}");
  Nb_R  = new  InvisibleRecoFrame ("Nb_R","#nu_{b}");

  LAB_R->SetChildFrame(*H_R);
  H_R ->AddChildFrame(*Wa_R);
  H_R ->AddChildFrame(*Wb_R);
  Wa_R->AddChildFrame(*La_R);
  Wa_R->AddChildFrame(*Na_R);
  Wb_R->AddChildFrame(*Lb_R);
  Wb_R->AddChildFrame(*Nb_R);

  if( ! LAB_R->InitializeTree()){ return EL::StatusCode::FAILURE; }

  // define groups for the reconstruction trees
  INV_R = new InvisibleGroup("INV_R","WIMP Jigsaws");
  INV_R->AddFrame(*Na_R);
  INV_R->AddFrame(*Nb_R);

  // define jigsaws for the reconstruction tree
  MinMassJigsaw_R = new SetMassInvJigsaw ("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R->AddJigsaw(*MinMassJigsaw_R);

  RapidityJigsaw_R = new SetRapidityInvJigsaw ("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R->AddJigsaw(*RapidityJigsaw_R);
  RapidityJigsaw_R->AddVisibleFrames((LAB_R->GetListVisibleFrames()));

  ContraBoostJigsaw_R = new ContraBoostInvJigsaw("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R->AddJigsaw(*ContraBoostJigsaw_R);

  ContraBoostJigsaw_R->AddVisibleFrames((Wa_R->GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R->AddVisibleFrames((Wb_R->GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R->AddInvisibleFrames((Wa_R->GetListInvisibleFrames()), 0);
  ContraBoostJigsaw_R->AddInvisibleFrames((Wb_R->GetListInvisibleFrames()), 1);

  // check reconstruction trees
  if( ! LAB_R->InitializeAnalysis()) { return EL::StatusCode::FAILURE;}

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_lvlv::doCalculate(std::unordered_map<std::string, double>& RJVars,
						   xAOD::IParticleContainer& particles,
						   xAOD::MissingET& met
						   ){
  if( particles.size() < 2 ){return EL::StatusCode::SUCCESS;}//todo figure out if this how we should handle this case
  //todo do we need to require exactly 2 ?


  //todo check the initialization
  // give the signal-like tree the event info and analyze
  TVector3 vecMet(met.mpx() , met.mpy(), 0.);

  std::vector<TLorentzVector> vecParticles;

  for(auto particle : particles){
    if( particle->type() != xAOD::Type::Electron &&
	particle->type() != xAOD::Type::Muon
	) {
      std::cout << "You passed a nonlepton to the lvlv calculator!!!" ;
      return EL::StatusCode::FAILURE;
    }//todo use the ATH messaging if possible??
    TLorentzVector tmpPart;
    tmpPart.SetPtEtaPhiE(particle->pt(),
			 particle->eta(),
			 particle->phi(),
			 particle->e()
			 );
    vecParticles.push_back(tmpPart);
  }

  auto ptSort = [](TLorentzVector const & a , TLorentzVector const & b){return a.Pt() < b.Pt();};
  std::sort(vecParticles.begin(),vecParticles.end(), ptSort);
  assert(vecParticles.at(0).Pt() > vecParticles.at(1).Pt());

  INV_R->SetLabFrameThreeVector(vecMet);               // Set the MET in reco tree
  La_R->SetLabFrameFourVector(vecParticles.at(0));
  Lb_R->SetLabFrameFourVector(vecParticles.at(1));
  LAB_R->AnalyzeEvent();                            // analyze the event

  //////////////////////////////////////
  // Observable Calculations
  //////////////////////////////////////

  double const MH = H_R->GetMass();
  double const MW = Wa_R->GetMass();

  RJVars["mH"] = MH;
  RJVars["mW"] = MW;

  return EL::StatusCode::SUCCESS;
}
