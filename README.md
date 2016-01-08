Once you have the RJigsawTools package with

```
git clone https://github.com/rsmith54/RJigsawTools.git
```

you'll still need a few dependencies. You'll need to get `Ext_RestFrames` and `CommonTools`.  `CommonTools` also depends on `QuickAna`.
First setup RootCore :

```
source RJigsawTools/util/setup.sh
```

Now we can checkout our dependencies using the following :

```
git clone https://github.com/lawrenceleejr/Ext_RestFrames.git
rc checkout_pkg atlasoff/PhysicsAnalysis/TopPhys/QuickAna/tags QuickAna
rc checkout_pkg svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/LBNL/AnalysisToolsRunII/CommonTools/tags CommonTools
git clone https://github.com/UCATLAS/xAODAnaHelpers.git
```

And of course do a normal rc set up procedure

```
rc find_packages
rc compile
```

Then you can run a test with e.g.:

```
python RJigsawTools/util/run_lvlv.py  --doOverwrite --nevents 10 --verbosity debug --dataDir /afs/cern.ch/user/l/leejr/work/lvlv_datasets #[point to a location of a DxAOD]
```

PLEASE NOTE : The output will be in
```
submit_dir/data-TEST_OUTPUTNAME/*.root
```
by default when doing a test, where TEST_OUTPUTNAME is set in your submission script.  The file in submit_dir does not have your output trees!!!