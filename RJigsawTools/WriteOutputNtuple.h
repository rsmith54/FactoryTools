#ifndef RJigsawTools_WriteOutputNtuple_H
#define RJigsawTools_WriteOutputNtuple_H

#include <EventLoop/Algorithm.h>

class NtupManager;

class WriteOutputNtuple : public EL::Algorithm
{

private :
  NtupManager * m_ntupManager; //!

  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;

  std::string outputName;
  std::string regionName;
  std::string systName  ;


  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!




  // this is a standard constructor
  WriteOutputNtuple ();

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(WriteOutputNtuple, 1);
};

#endif
