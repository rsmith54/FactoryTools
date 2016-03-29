#ifndef RJIGSAW_CALCULATOR_COMPRESSED_H
#define RJIGSAW_CALCULATOR_COMPRESSED_H
//author : Lawrence Lee
//date   : February 2016

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

class RJigsawCalculator_compressed : public RJigsawCalculator {

public :
  RJigsawCalculator_compressed();
  virtual  ~RJigsawCalculator_compressed ();

private :

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
  ClassDef(RJigsawCalculator_compressed, 1);
};

#endif //RJIGSAW_CALCULATOR_COMPRESSED_H
