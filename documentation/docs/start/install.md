# Install the GeoModel Libraries and Tools

In this section, you will find instructions on how to **install** the latest **stable** version of GeoModel libraries and tools on different platforms. 

## Ubuntu 20.0.4 (focal, LTS) or 20.0.10 (groovy)

Add the geomodel repository to your source list: 

```
sudo add-apt-repository ppa:kaktusjoe/geomodel
sudo apt-get update
```
Install geomodel precompiled applications
```
sudo apt install geomodel-visualization  # to get gmex  (the geometry explorer)
sudo apt install fullsimlight            # to get fullsimlight,gm2gdml,gmclash, gmgeantino
sudo apt install geomodeltools           # to get gmcat, gdml2gm
```
Install the geomodel development kits (header files and precompiled libraries)
```
sudo apt install geomodelcore-dev
sudo apt install geomodeltools-dev
sudo apt install geomodelg4-dev
sudo apt install geomodel-visualization-dev
```
Get some example geometry input files:
```
sudo apt install geometry-data
```


## Install the GeoModel visualizer and all the libraries

By installing the GeoModelVisualization package, you will install the visualization tool as well as all the base GeoModel libraries and tools, and all ther dependencies.

Therefore, this is the **quickest**  way to get all the libraries and tools you will need to **create**, **store**, **restore**, **visualize**, and **debug** *detector description data*.

### macOS

On macOS, we use the Homebrew package manager to handle and install the GeoModel packages. If you don't have Homebrew installed on your machine, you should install it by following the instructions on the Homebrew website: <http://brew.sh>.

You can install the visualization tool and all the libraries by running the following commands:

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git 
brew update 
brew install geomodelvisualization  
```

The first command will set the Homebrew repository (a "Tap" in Homebrew jargon) storing the "recipes" to install the ATLAS GeoModel libraries on macOS; this command is only needed the first time you install the `atlas/geomodel` Tap. The second command makes sure that all latest versions are taken; this should be run all times you want to update or install a GeoModel package. The third command installs all the libraries, tools, and their dependencies.

**Pre-compiled packages** ("Bottles", in the Homebrew jargon) are provided for the **latest two** macOS release(s). That **speeds up** the installation a lot.

For older macOS releases, Homebrew will try to compile the packages from source. If that fails, you will have to compile from source by yourself by following the instructions provided in the [development](../dev/index.md) section.

!!! note

    Brew is usually smart enough to figure out that the `geomodelvisualization` formula belongs to the `atlas/geomodel` Tap. However, if that's not the case, or in case you have other packages with similar names on your system, you can install the GeoModel formulas by passing to `brew` their complete name; *e.g.*,

    ```
    brew install atlas/geomodel/geomodelvisualization
    ```

!!! tip
    
    If you experience problems in installing GeoModel packages with brew or using them, please try to clean your system from old packages or repositories. For that, please refer to the [Troubleshooting page](/dev/troubleshooting.md).



### Ubuntu

_Coming soon_

### Other platforms

On any other platform, you can build GeoModel from sources, following [these instructions](../dev/index.md)



## Visualize geometry data

After installinmg the tools, you can test your installation on an example ATLAS geometry file. 

First, download the geometry data file:

{!start/get-geometry-example-file.md!}

Now, you can interactively visualizing the geometry data by running:

```
gmex geometry_atlas.db
```



