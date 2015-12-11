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

#include "DibosonRJ/RJigsawCalculator_lvlv.h"

#include "EventLoop/StatusCode.h"

EL::StatusCode RJigsawCalculator_lvlv::clearEvent() {
  if(! LAB_G->ClearEvent()){return EL::StatusCode::FAILURE;}
  if(! LAB_R->ClearEvent()){return EL::StatusCode::FAILURE;}

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode RJigsawCalculator_lvlv::initialize() {
  using namespace RestFrames;

  // Set up toy generation tree (not needed for reconstruction)
  LAB_G = new LabGenFrame      ("LAB_G","LAB");
  H_G   = new ResonanceGenFrame("H_G","H");
  Wa_G  = new ResonanceGenFrame ("Wa_G","W_{a}");
  Wb_G  = new ResonanceGenFrame("Wb_G","W_{b}");
  La_G  = new VisibleGenFrame  ("La_G","#it{l}_{a}");
  Na_G  = new InvisibleGenFrame ("Na_G","#nu_{a}");
  Lb_G  = new VisibleGenFrame   ("Lb_G","#it{l}_{b}");
  Nb_G  = new InvisibleGenFrame ("Nb_G","#nu_{b}");

  LAB_G->SetChildFrame(*H_G);
  H_G->AddChildFrame(*Wa_G);
  H_G->AddChildFrame(*Wb_G);
  Wa_G->AddChildFrame(*La_G);
  Wa_G->AddChildFrame(*Na_G);
  Wb_G->AddChildFrame(*Lb_G);
  Wb_G->AddChildFrame(*Nb_G);

  if( ! LAB_G->InitializeTree()){ return EL::StatusCode::FAILURE;}
  // set Higgs masses

  //todo fix
  double const mH  = 125.;
  double const mHw = 1. ;

  double const mW  = 80.;
  double const mWw = 3  ;

  double const mL = .105;
  double const mN = 0.;

  H_G->SetMass (mH  );
  H_G->SetWidth(mHw);
  // set W masses
  Wa_G->SetMass(mW);
  Wb_G->SetMass(mW);
  Wa_G->SetWidth(mWw);
  Wb_G->SetWidth(mWw);
  // set lepton masses
  La_G->SetMass(mL);
  Lb_G->SetMass(mL);
  // set neutrino masses
  Na_G->SetMass(mN);
  Nb_G->SetMass(mN);

  if( ! LAB_G->InitializeAnalysis()){ return EL::StatusCode::FAILURE;}

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

EL::StatusCode RJigsawCalculator_lvlv::calculate(std::unordered_map<std::string, double>& RJVars,
						 xAOD::IParticleContainer particles
						 ){



  return EL::StatusCode::SUCCESS;
}
