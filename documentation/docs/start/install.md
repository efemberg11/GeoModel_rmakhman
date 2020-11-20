# Install the GeoModel Libraries

In this section, instructions on how to **install** the GeoModel libraries on different platforms are given. The instructions are aimed at **end users**. A relevant package manager will be used for each of the **supported platforms**, and the commands will install **stable**, **pre-compiled** versions of the libraries.

However, if you would like to:

- build the libraries for an unsupported platform
- get the latest version of the code from the GeoModel repository
- develop the libraries

then, you have to **compile them** from scratch. In that case, please follow the instructions provided in the [For Developers](../dev/index.md) section.

If you have problems, please send a message to the mailing list provided in the [Contacts](../contacts.md) page.


## Install the GeoModel visualizer and all the libraries

By installing the GeoModelVisualization package, you will install the visualization tool as well as all the base GeoModel libraries and tools, and all the dependencies.

Therefore, that is the quickest and simplest way to get all the libraries and tools you will need to create, store, restore, visualize, and debug detector description data.

### macOS

On macOS, we use the Homebrew package manager to handle and install the GeoModel packages. If you don't have Homebrew installed on your machine, you should install it by following the instructions on the Homebrew website: <http://brew.sh>.

You can install the visualization tool and all the libraries by running the following commands:

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install geomodelvisualization
```

The first command will set the Homebrew repository (a "Tap" in Homebrew jargon) storing the "recipes" to install the ATLAS GeoModel libraries on macOS. The second command install all the libraries.

Pre-compiled packages ("Bottles", in the Homebrew jargon) are provided for the latest macOS release(s). That speeds up the installation a lot.

For older macOS releases, Homebrew will try to compile the packages from source. If that fails, you will have to compile from source by yourself by following the instructions provided in the [dev](/dev/index.md) section.

In case you want to install only the base libraries and tools (without visualization), you can install them (after having *tapped* the repository as before) with:

```
brew install geomodel
```

!!! note

    Brew is usually smart enough to figure out that the `geomodelvisualization` formula belongs to the `atlas/geomodel` Tap. Hoewever, if that's not the case, or in case you have other packages with similar names on your system, you can install the GeoModel formulas by passing to `brew` their complete name; *e.g.*,

    ```
    brew install atlas/geomodel/geomodelvisualization
    ```

    or 

    ```
    brew install atlas/geomodel/geomodel
    ```

!!! warning

    At the time of writing this note, Homebrew is not fully supporting macOS 11 Big Sur yet. On that OS you might need to install the `geomodel` package explicitely together with `geomodelvisualization` without relying on the usual ability of Homebrew to install all the needed dependency packages. Thus, you might need to run:

    ```
    brew install geomodel geomodelvisualization
    ```

!!! tip
    
    If you experience problems in installing GeoModel packages with brew or using them, please try to clean your system from old packages or repositories. For that, please refer to the [Troubleshooting page](dev/troubleshooting.md).



### Ubuntu

_Coming soon_

### Centos7 / CC7

_Coming soon_

### SLC6

_Coming soon_
