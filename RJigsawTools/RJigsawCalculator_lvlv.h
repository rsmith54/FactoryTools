#ifndef RJIGSAW_CALCULATOR_LVLV_H
#define RJIGSAW_CALCULATOR_LVLV_H
//author : Russell Smith
//date   : December 2015

#include "RJigsawTools/RJigsawCalculator.h"

namespace RestFrames {
  class LabGenFrame;
  class ResonanceGenFrame;
  class ResonanceGenFrame;
  class ResonanceGenFrame;
  class VisibleGenFrame;
  class InvisibleGenFrame;
  class VisibleGenFrame;
  class InvisibleGenFrame;


  class LabRecoFrame ;
  class DecayRecoFrame ;
  class DecayRecoFrame ;
  class DecayRecoFrame ;
  class VisibleRecoFrame ;
  class InvisibleRecoFrame ;
  class VisibleRecoFrame ;
  class InvisibleRecoFrame ;

  class InvisibleGroup;
  class CombinatoricGroup;
  class MinMassesCombJigsaw;
  class SetMassInvJigsaw;
  class SetRapidityInvJigsaw;
  class ContraBoostInvJigsaw;
}

class RJigsawCalculator_lvlv : public RJigsawCalculator {

public :
  RJigsawCalculator_lvlv();
  virtual  ~RJigsawCalculator_lvlv ();

private :

  RestFrames::LabRecoFrame * LAB_R;
  RestFrames::DecayRecoFrame * H_R;
  RestFrames::DecayRecoFrame * Wa_R;
  RestFrames::DecayRecoFrame * Wb_R;
  RestFrames::VisibleRecoFrame * La_R;
  RestFrames::InvisibleRecoFrame * Na_R;
  RestFrames::VisibleRecoFrame * Lb_R;
  RestFrames::InvisibleRecoFrame * Nb_R;

  RestFrames::InvisibleGroup       * INV_R;
  RestFrames::CombinatoricGroup    * VIS_R;
  RestFrames::MinMassesCombJigsaw  * HemiJigsaw_R;
  RestFrames::SetMassInvJigsaw     * MinMassJigsaw_R;
  RestFrames::SetRapidityInvJigsaw * RapidityJigsaw_R;
  RestFrames::ContraBoostInvJigsaw * ContraBoostJigsaw_R;

private :
  virtual EL::StatusCode doInitialize();
  virtual EL::StatusCode doClearEvent();
  virtual EL::StatusCode doCalculate(std::map<std::string, double>& RJVars,
                                     xAOD::IParticleContainer& particles,
                                     xAOD::MissingET& met
                                     );
  // this is needed to distribute the algorithm to the workers
  ClassDef(RJigsawCalculator_lvlv, 1);
};

#endif //RJIGSAW_CALCULATOR_LVLV_H
