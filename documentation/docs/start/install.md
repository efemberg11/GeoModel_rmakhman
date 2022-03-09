# Install the GeoModel Applications and Development kits

In this section, you will find instructions on how to **install** the latest **stable** version of GeoModel applications and development kits on different platforms.  The GeoModel applications include

* **gmex**, the geometry explorer, an interactive 3d geometry visualizer
* **fullsimlight**, a command-line tool to run Geant4 simulation.
* **gmclash**, a Geant4 based command-line tool too perform clash detection
* **gmgeantino**, a Geant4 based command-line tool to generate geantino maps
* **gm2gdml** and **gdml2gm**, command line tools to convert GeoModel to and from GDML
* **gmcat**, a command line tool to assemble geometry from multiple inputs

The development kits include header files and precompiled libraries for the development of detector description code. On some platforms (Mac) the development environment is installed together with the prebuild applications and libraries.  


## Ubuntu 20.0.4 (focal, LTS) or 20.0.10 (groovy)

Add the geomodel repository to your source list: 

```
sudo add-apt-repository ppa:kaktusjoe/geomodel
sudo apt-get update
```
Install geomodel precompiled applications
```
sudo apt install geomodel-visualization  # to get gmex (the geometry explorer)
sudo apt install fullsimlight            # to get fullsimlight, gm2gdml, gmclash, gmgeantino
sudo apt install geomodel-tools          # to get gmcat, gdml2gm
```
Install the geomodel development kits (header files and precompiled libraries)
```
sudo apt install geomodel-core-dev
sudo apt install geomodel-tools-dev
sudo apt install geomodel-g4-dev
sudo apt install geomodel-visualization-dev
```



## macOS

On macOS, we use the Homebrew package manager to handle and install the GeoModel packages. If you don't have Homebrew installed on your machine, you should install it by following the instructions on the Homebrew website: <http://brew.sh>.

### First installation 

At first, you add the dedicated Homebrew repository:

```shell
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git 
brew update 
```

The first command will set the Homebrew repository (a *Tap*, in Homebrew jargon) storing the "recipes" to install the GeoModel libraries on macOS; this command is only needed the first time you install the `atlas/geomodel` Tap. The second command makes sure that all latest versions are taken; this should be run all times you want to update or install a GeoModel package.

Then, you install the pre-compiled GeoModel applications:

```shell
brew install geomodel-visualization # to get gmex (the geometry explorer)
brew install geomodel-fullsimlight  # to get fullsimlight, gm2gdml, gmclash, gmgeantino
brew install geomodel-tools         # to get gmcat, gdml2gm
```



The use of **Pre-compiled packages** (*Bottles*, in the Homebrew jargon) **speeds up** the installation on the user's machine a lot. GeoModel bottles are provided at least for the **latest two** macOS releases.

For older macOS releases, Homebrew will try to compile the packages from source. If that fails, you will have to compile from source by yourself by following the instructions provided in the [development](../dev/index.md) section.

!!! note

    Homebrew is usually smart enough to figure out that the `geomodelvisualization` formula belongs to the `atlas/geomodel` Tap. However, if that's not the case, or in case you have other packages with similar names on your system, you can install the GeoModel formulas by passing to `brew` their complete name; *e.g.*,

    ```
    brew install atlas/geomodel/geomodelvisualization
    ```

!!! tip
    
    If you experience problems in installing GeoModel packages with brew or using them, please try to clean your system from old packages or repositories. For that, please refer to the [Troubleshooting page](../dev/troubleshooting.md).


### Update

#### Quick instructions 

As soon as you have followed the initial installation instructions above, you can update the GeoModel libraries and tools to the latest version simply by running the commands here below:

```shell
brew update
brew upgrade geomodel geomodel-geomodeltools geomodel-visualization geomodel-geomodelg4 geomodel-fullsimlight
```

#### Some more details 

After the first command, `brew update`, you get a summary of the updated repositories and recipes. If there are new versions of the GeoModel tools and libraries, you should see something like the output messages here below:

```
➜  ~ brew update
Updated Homebrew from e13d04c1d to 784be839b.
Updated 5 taps (homebrew/core, homebrew/cask, homebrew/bundle, homebrew/services and atlas/geomodel).
...
==> Updated Formulae
...
atlas/geomodel/geomodel                     fbthrift                                    libvdpau                                    sdcc
atlas/geomodel/geomodel-fullsimlight        feroxbuster                                 lilv                                        serverless
atlas/geomodel/geomodel-geomodelg4          fheroes2                                    localstack                                  snakemake
atlas/geomodel/geomodel-tools               fizz                                        lua-language-server                         snort
atlas/geomodel/geomodel-visualization
...
```

You can now update the GeoModel recipes with `brew upgrade ...`. 

Please notice that, if you get an error like the one below:

```shell
➜  ~ brew upgrade geomodel
Error: atlas/geomodel/geomodel not installed
```

that means that, even if the GeoModel repository (`tap`) has been installed, at a certain point the `geomodel` Homebrew package has been removed (for example, to test other versions). Therefore, the `upgrade` command cannot work. You should re-install the package with `brew install geomodel`. The same with all the other GeoModel Homebrew packages.





## Installation from source code.

On any other platform, you can build GeoModel from sources, following [these instructions](../dev/index.md)



## Visualize geometry data

After installinmg the tools, you can test your installation on an example ATLAS geometry file. 

First, download the geometry data file:

{!start/get-geometry-example-file.md!}

Now, you can interactively visualizing the geometry data by running:

```
gmex geometry_atlas.db
```


## Uninstall

If you want to uninstall GeoModel libraries and applications, you can follow the instructions below

### macOS

If you installed GeoModel by using Homebrew as suggested above, you can easily uninstall all the GeoModel packages with a single `brew` command:

```
brew remove geomodel geomodel-tools geomodel-visualization geomodel-geomodelg4 geomodel-fullsimlight geomodel-thirdparty-simage geomodel-thirdparty-coin geomodel-thirdparty-soqt
```






