#ifndef RJIGSAW_CALCULATOR_LVLV_H
#define RJIGSAW_CALCULATOR_LVLV_H
//author : Russell Smith
//date   : December 2015

#include "EventLoop/StatusCode.h"
#include "xAODBase/IParticleContainer.h"

#include <unordered_map>

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
  class SetMassInvJigsaw;
  class SetRapidityInvJigsaw;
  class ContraBoostInvJigsaw;
}

class RJigsawCalculator_lvlv {

private :
  RestFrames::LabGenFrame *      LAB_G;
  RestFrames::ResonanceGenFrame *H_G;
  RestFrames::ResonanceGenFrame *Wa_G;
  RestFrames::ResonanceGenFrame *Wb_G;
  RestFrames::VisibleGenFrame *  La_G;
  RestFrames::InvisibleGenFrame *Na_G;
  RestFrames::VisibleGenFrame *  Lb_G;
  RestFrames::InvisibleGenFrame *Nb_G;

  RestFrames::LabRecoFrame * LAB_R;
  RestFrames::DecayRecoFrame * H_R;
  RestFrames::DecayRecoFrame * Wa_R;
  RestFrames::DecayRecoFrame * Wb_R;
  RestFrames::VisibleRecoFrame * La_R;
  RestFrames::InvisibleRecoFrame * Na_R;
  RestFrames::VisibleRecoFrame * Lb_R;
  RestFrames::InvisibleRecoFrame * Nb_R;

  RestFrames::InvisibleGroup       * INV_R;
  RestFrames::SetMassInvJigsaw     * MinMassJigsaw_R;
  RestFrames::SetRapidityInvJigsaw * RapidityJigsaw_R;
  RestFrames::ContraBoostInvJigsaw * ContraBoostJigsaw_R;

public :

  EL::StatusCode initialize();

  //to be used per event
  EL::StatusCode clearEvent();
  EL::StatusCode calculate(std::unordered_map<std::string, double>& RJVars,
			   xAOD::IParticleContainer particles
			   );

};

#endif //RJIGSAW_CALCULATOR_LVLV_H
