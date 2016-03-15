#include <RJigsawTools/CalculateRegionVars.h>

#include <RJigsawTools/CalculateRJigsawVariables.h>


#include <RJigsawTools/PreselectDileptonicWWEvents.h>
#include <RJigsawTools/SelectDileptonicWWEvents.h>
#include <RJigsawTools/PostselectDileptonicWWEvents.h>

#include <RJigsawTools/PreselectTwoLeptonEvents.h>
#include <RJigsawTools/SelectTwoLeptonEvents.h>
#include <RJigsawTools/PostselectTwoLeptonEvents.h>

//#include <RJigsawTools/PreselectDileptonicWWEvents.h>
#include <RJigsawTools/SelectZeroLeptonEvents.h>
//#include <RJigsawTools/PostselectDileptonicWWEvents.h>


#include <RJigsawTools/PreselectDiFatJetEvents.h>
#include <RJigsawTools/SelectDiFatJetEvents.h>

#include <RJigsawTools/RegionVarCalculator.h>
#include <RJigsawTools/RegionVarCalculator_lvlv.h>
#include <RJigsawTools/RegionVarCalculator_zl.h>
#include <RJigsawTools/RegionVarCalculator_tls.h>
#include <RJigsawTools/RegionVarCalculator_b4j.h>

#include <RJigsawTools/RJigsawCalculator.h>
#include <RJigsawTools/RJigsawCalculator_compressed.h>
#include <RJigsawTools/RJigsawCalculator_lvlv.h>
#include <RJigsawTools/RJigsawCalculator_zl.h>
#include <RJigsawTools/RJigsawCalculator_tls.h>

#include <RJigsawTools/WriteOutputNtuple.h>
#include <RJigsawTools/CalibrateST.h>
#include <RJigsawTools/MCEventVeto.h>
#include <RJigsawTools/MCEventVetoHelper.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__
#pragma link C++ class CalibrateST+;
#endif

#ifdef __CINT__
#pragma link C++ class MCEventVeto+;
#endif

#ifdef __CINT__
#pragma link C++ class MCEventVetoHelper+;
#endif


#ifdef __CINT__
#pragma link C++ class RJigsawCalculator+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_lvlv+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_compressed+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_zl+;
#endif

#ifdef __CINT__
#pragma link C++ class RJigsawCalculator_tls+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_lvlv+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_zl+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_tls+;
#endif

#ifdef __CINT__
#pragma link C++ class RegionVarCalculator_b4j+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectZeroLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PreselectDileptonicWWEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectDileptonicWWEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PreselectDiFatJetEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectDiFatJetEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PostselectDileptonicWWEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PreselectTwoLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class SelectTwoLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class PostselectTwoLeptonEvents+;
#endif

#ifdef __CINT__
#pragma link C++ class CalculateRJigsawVariables+;
#endif

#ifdef __CINT__
#pragma link C++ class WriteOutputNtuple+;
#endif

#ifdef __CINT__
#pragma link C++ class CalculateRegionVars+;
#endif
