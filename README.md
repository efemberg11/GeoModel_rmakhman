# GeoModelXml

This package contains a modernised version of Nigel Hessey's GeoModelXml to be 
used with `gmex` (GeoModelExplorer) and the corresponding plugin (GMXPlugin)

## Quick Instructions

### How to clone it

From e.g. your $HOME directory

```
git clone https://gitlab.cern.ch/GeoModelATLAS/geomodelxml.git
```

### How to build it

```
mkdir build_gmx
cd build_gmx
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ../geomodelxml
make install
```

### How to run it

Running the GMXPlugin executes the xerces parser to parse the xml file. The expression evaluator only works if the system language is set to USEnglish, thus do:
```bash
export LANG=en_US.UTF-8
```

From e.g. your home directory

```
mkdir run ; cd run
cp ../geomodelxml/GMXPlugin/data/* .
ln -s HelloWorld.gmx gmx.xml
gmex ../install/lib/libGMXPlugin.so  (or .dylib on mac)
```
 
## Step-by-step Instructions - macOS

By following the steps here below, you will install GeoModelXML on top of the full software stack needed to build and run it.
Feel free to skip steps `a)-c)` if you have the GeoModel classes installed on your platform already.

```
# Step a) - Clone and build GeoModelCore from source
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelCore.git
mkdir build_GeoModelCore; cd build_GeoModelCore
cmake ../GeoModelCore
make -j
sudo make install
cd ..
# Step b) - Clone and build GeoModelIO
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelIO.git
mkdir build_GeoModelIO; cd build_GeoModelIO
cmake ../GeoModelIO
make -j
sudo make install
cd ..

# Step c) - Clone geomodelvisualization and build gmex
git clone https://gitlab.cern.ch/GeoModelDev/geomodelvisualization.git
mkdir build_geomodelvisualization; cd build_geomodelvisualization
cmake ../geomodelvisualization
make -j
sudo make install
cd ..


# Step 0 - Create a working directory 
mkdir GeoModel_work; cd GeoModel_work

# Step 1 - Install XercesC
brew install xerces-c

# Step 2 - Install Eigen3
brew install eigen

# Step 3 - Clone and build the `nlohmann_json` parser
git clone https://github.com/nlohmann/json.git
mkdir build_json; cd build_json
cmake ../json
sudo make install
cd ..

# Step 4 - Install GeoModelTools and build it
git clone https://gitlab.cern.ch/GeoModelDev/GeoModelTools.git
mkdir build_GeoModelTools; cd build_GeoModelTools
cmake ../GeoModelTools
make -j
sudo make install
cd ..

# Step 5 - Clone GeoModelXml and build it
git clone https://gitlab.cern.ch/GeoModelATLAS/geomodelxml.git
mkdir build_geomodelxml; cd build_geomodelxm 
cmake ../geomodelxml
make -j
sudo make install
cd ..

# Step 6 - Run gmex with GMXPlugin and build the ITk strips
mkdir run; cd run
cp ../geomodelxml/GMXPlugin/data/* .
ln -s StripBlobRev_5433bf5.xml gmx.xml
```

Then, you can run `gmex` (GeoModelExplorer) with:

```
gmex /usr/local/lib/libGMXPlugin.dylib
```

Once `gmex` is open, click on the **Geo** tab, then check **SCT**



