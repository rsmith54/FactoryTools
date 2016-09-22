## Getting Started

Once you have the FactoryTools package with

```
git clone https://github.com/rsmith54/FactoryTools.git #!
```

you'll still need a few dependencies. You'll need to get `Ext_RestFrames` and `CommonTools`. We are using a newer version of SUSYTools than is currently in the release, while depending on a package not in the release we are using yet (todo update when not true anymore).
First setup RootCore :

```
source FactoryTools/util/setup.sh #!
```

Now we can checkout our dependencies using the following :

```
rc checkout_pkg svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-08-05 SUSYTools; #!!
rc checkout SUSYTools/doc/packages.txt; #!!
git clone https://github.com/UCATLAS/xAODAnaHelpers.git; cd xAODAnaHelpers; git checkout 630919c55078d543f91e1dee12175a1c4783941e;  git checkout -b localRJbranch; cd ..; #!!
git clone https://github.com/lawrenceleejr/Ext_RestFrames.git; cd Ext_RestFrames; git checkout v1.7; git checkout -b v1.7; cd ..; #!!
rc checkout_pkg svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/LBNL/AnalysisToolsRunII/CommonTools/trunk/ CommonTools; cd CommonTools; svn update -r 417790; cd -; #!!
```

And of course do a normal rc set up procedure

```
rc find_packages
rc compile
```

Then you can run a test with e.g.:

```
python FactoryTools/util/run_zl.py  --doOverwrite --nevents 10 --verbosity debug --inputDS /afs/cern.ch/work/r/rsmith/public/factoryToolsSample/
```

(This points to a public directory so this should run for anyone on afs.)

The inputDS option is smart.  You can give supply a local directory, txt file with a list of grid datasets, or a pattern which matches a grid pattern.  If you use the grid options, obviously you need panda stuff setup, which is setup by the setup script.

PLEASE NOTE : The output will be in
```
submit_dir/data-TEST_OUTPUTNAME/*.root
```
by default when doing a test, where TEST_OUTPUTNAME is set in your submission script.  The file in submit_dir does not have your output trees!!!


To run on the gird, first setup panda :
```
lsetup panda
```

And then you can call

```
python FactoryTools/util/run_zl.py --doOverwrite --driver grid --inputDS FactoryTools/data/mc15c_13TeV_SM_SUSY2_p2613.ds
```

This default assumes your system username is the same as your grid/NICE name. If not, you can change it with --gridUser, and the --gridTag defaults to just the date.

```
# Please keep #! markers for install commands
# #!! markers are used by CI for gitlab compilation
```

## Merging Step

Once you've run on the grid, download the tree output and the metadata output to some directory. Then you can use the new merging script to combine them with a new branch added that incorporates the cross section weighting. Simply run

```
python mergeOutput.py --inDir [path to datasets from grid]
```

And it will produce combined files containing the trees. One just needs to weight by the branch normweight. The assignment of dataset types is done in discoverInput.py where tags are added to samples based on their names. Then all of one kind of process are combined.
