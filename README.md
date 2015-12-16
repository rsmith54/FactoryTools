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
rc checkout_pkg svn+ssh://rsmith@svn.cern.ch/reps/atlasinst/Institutes/LBNL/AnalysisToolsRunII/CommonTools/tags CommonTools
```




