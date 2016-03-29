#ifndef RJIGSAW_CALCULATOR_ZL_H
#define RJIGSAW_CALCULATOR_ZL_H
//author : Russell Smith
//date   : January 2015

#include "RJigsawTools/RJigsawCalculator.h"

namespace RestFrames {
  class LabRecoFrame ;
  class DecayRecoFrame ;
  class SelfAssemblingRecoFrame ;
  class VisibleRecoFrame ;
  class InvisibleRecoFrame ;

  class InvisibleGroup ;
  class CombinatoricGroup     ;
  class SetMassInvJigsaw       ;
  class SetRapidityInvJigsaw   ;
  class ContraBoostInvJigsaw   ;
  class MinMassesCombJigsaw    ;
  class SetRapidityInvJigsaw   ;
}

class RJigsawCalculator_zl : public RJigsawCalculator {

public :
  RJigsawCalculator_zl();
  virtual  ~RJigsawCalculator_zl ();

private :
  RestFrames::LabRecoFrame *  LAB;
  RestFrames::DecayRecoFrame *  PP;
  RestFrames::DecayRecoFrame *  Pa;
  RestFrames::DecayRecoFrame *  Pb;
  RestFrames::DecayRecoFrame *  Ca;
  RestFrames::DecayRecoFrame *  Cb;
  RestFrames::SelfAssemblingRecoFrame *  SAV1a;
  RestFrames::SelfAssemblingRecoFrame * SAV1b;
  RestFrames::SelfAssemblingRecoFrame * SAV2a;
  RestFrames::SelfAssemblingRecoFrame * SAV2b;
  RestFrames::VisibleRecoFrame * V1a;
  RestFrames::VisibleRecoFrame * V1b;
  RestFrames::VisibleRecoFrame * V2a;
  RestFrames::VisibleRecoFrame * V2b;
  RestFrames::InvisibleRecoFrame * Ia;
  RestFrames::InvisibleRecoFrame * Ib;

  RestFrames::InvisibleGroup         *      INV              ;
  RestFrames::CombinatoricGroup      *      VIS              ;
  RestFrames::SetMassInvJigsaw       *      InvMassJigsaw    ;
  RestFrames::SetRapidityInvJigsaw   *      InvRapidityJigsaw;
  RestFrames::ContraBoostInvJigsaw   *      InvCBJigsaw      ;
  RestFrames::MinMassesCombJigsaw    *      CombPPJigsaw     ;
  RestFrames::MinMassesCombJigsaw    *      CombPaJigsaw     ;
  RestFrames::MinMassesCombJigsaw    *      CombPbJigsaw     ;
  RestFrames::LabRecoFrame           *     LAB_bkg          ;
  RestFrames::SelfAssemblingRecoFrame*     S_bkg            ;
  RestFrames::VisibleRecoFrame       *     V_bkg            ;
  RestFrames::InvisibleRecoFrame     *     I_bkg            ;
  RestFrames::InvisibleGroup         *     INV_bkg          ;
  RestFrames::CombinatoricGroup      *     VIS_bkg          ;
  RestFrames::SetMassInvJigsaw       *     InvMass_bkg      ;
  RestFrames::SetRapidityInvJigsaw   *     InvRapidity_bkg  ;

private :
  virtual EL::StatusCode doInitialize();
  virtual EL::StatusCode doClearEvent();
  virtual EL::StatusCode doCalculate(std::map<std::string, double>& RJVars,
                                     xAOD::IParticleContainer& particles,
                                     xAOD::MissingET& met
                                     );
  // this is needed to distribute the algorithm to the workers
  ClassDef(RJigsawCalculator_zl, 1);
};

#endif //RJIGSAW_CALCULATOR_ZL_H
