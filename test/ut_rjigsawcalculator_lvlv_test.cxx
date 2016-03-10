//author : Russell Smith
//date   : December 2015
//adapted from Code from Nils Krumnack

#include <RJigsawTools/RJigsawCalculator_lvlv.h>

#include <RootCoreUtils/Assert.h>
#include <TSystem.h>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>

#include <map>
#include <xAODMissingET/MissingET.h>
#include <xAODJet/JetContainer.h>
#include <xAODParticleEvent/ParticleContainer.h>

struct RJigsawCalculator_lvlv_Test : testing::Test
{
  static const std::string root;

  virtual void SetUp ()
  {
    // gSystem->MakeDirectory ((root + "").c_str());
    // std::ofstream ((root + "/file.root_17").c_str());
    // std::ofstream ((root + "/ignore").c_str());
    // gSystem->MakeDirectory ((root + "/sample").c_str());
    // std::ofstream ((root + "/sample/samplefile.root").c_str());
    // gSystem->MakeDirectory ((root + "/deepsample").c_str());
    // gSystem->MakeDirectory ((root + "/deepsample/subdir").c_str());
    // std::ofstream ((root + "/deepsample/subdir/deepfile.root").c_str());
  }

  virtual void TearDown(){}
};
const std::string RJigsawCalculator_lvlv_Test::root = "test1";

TEST_F (RJigsawCalculator_lvlv_Test, clearEventTest)
{
  RJigsawCalculator_lvlv calc;
  calc.initialize();

  std::map<std::string, double> mymap;
  xAOD::ParticleContainer jets;
  xAOD::MissingET met;


  EXPECT_TRUE( calc.calculate(mymap, jets, met) == EL::StatusCode::SUCCESS ) ;//the first event is fine no matter what
  EXPECT_TRUE( calc.clearEvent() == EL::StatusCode::SUCCESS );//clear event should work
  EXPECT_TRUE( calc.calculate(mymap, jets, met) == EL::StatusCode::SUCCESS ) ;//after clearing everything works fine
  EXPECT_TRUE( calc.calculate(mymap, jets, met) == EL::StatusCode::FAILURE ) ;//calling calculate twice fails!
}

int main (int argc, char **argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS();
}
