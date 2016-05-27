#ifndef RJIGSAW_CALCULATOR_H
#define RJIGSAW_CALCULATOR_H
//author : Russell Smith
//date   : December 2015

#include "EventLoop/StatusCode.h"
#include "xAODBase/IParticleContainer.h"
//#include "xAODParticleEvent/ParticleContainer.h"
#include "xAODMissingET/MissingET.h"

#include <map>
#include <iostream>

class RJigsawCalculator {

public :
  virtual ~RJigsawCalculator() = 0;


private :
  enum ClearEventCalled {
    NOTCALLED = 0,
    CALLED = 1
  };

  ClearEventCalled  m_clearEventCalled ;

public :
  EL::StatusCode initialize(){m_clearEventCalled = CALLED;return doInitialize();}
  //to be used per event
  EL::StatusCode clearEvent(){m_clearEventCalled = CALLED;return doClearEvent();}
  EL::StatusCode calculate(std::map<std::string, double>& RJVars,
			   xAOD::IParticleContainer& particles,
			   xAOD::MissingET & met
			   ){
    if(m_clearEventCalled == NOTCALLED){
      std::cout << "You didn't call clearEvent ! Exiting." << std::endl;
      return EL::StatusCode::FAILURE;
    }
    m_clearEventCalled = NOTCALLED;
    return doCalculate( RJVars,
			particles,
			met);
  }

private :
  //todo probably clean this up
  virtual EL::StatusCode doInitialize(){std::cout << "you called the base calculator function! Exiting" << std::endl;return EL::StatusCode::FAILURE;};
  virtual EL::StatusCode doClearEvent(){std::cout << "you called the base calculator function! Exiting" << std::endl;return EL::StatusCode::FAILURE;};
  virtual EL::StatusCode doCalculate(std::map<std::string, double>& RJVars,
                                     xAOD::IParticleContainer& particles,
                                     xAOD::MissingET& met
                                     ){std::cout << "you called the base calculator function! Exiting" << std::endl;return EL::StatusCode::FAILURE;};


public :
  // this is needed to distribute the algorithm to the workers
  ClassDef(RJigsawCalculator, 1);

};

inline RJigsawCalculator::~RJigsawCalculator() { }

#endif //RJIGSAW_CALCULATOR_H
