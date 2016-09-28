#ifndef FactoryTools_CalibrateST_H
#define FactoryTools_CalibrateST_H

#include <EventLoop/Algorithm.h>
#include "SUSYTools/SUSYObjDef_xAOD.h"

#include "PATInterfaces/SystematicSet.h"


class CalibrateST : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  const std::string& notSetString() {
    static std::string const notSetString = "notSet";
    return notSetString;
  }

  CP::SystematicSet systVar;
  std::string SUSYToolsConfigFileName;
  std::string PRWConfigFileNames;
  std::string PRWLumiCalcFileNames;
  std::string fatJetContainerName;

  std::string muTrig2015;
  std::string muTrig2016;
  std::string elTrig2015;
  std::string elTrig2016;


  ST::SUSYObjDef_xAOD *m_objTool;//!

  // this is a standard constructor
  CalibrateST ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  EL::StatusCode addToSystList(std::string listName, std::string systName);

  // this is needed to distribute the algorithm to the workers
  ClassDef(CalibrateST, 1);
};

#endif
