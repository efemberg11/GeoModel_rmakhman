# Install the GeoModel Libraries

In this section, instructions on how to **install** the GeoModel libraries on different platforms are given. The instructions are aimed at **end users**. A relevant package manager will be used for each of the **supported platforms**, and the commands will install **stable**, **pre-compiled** versions of the libraries.

Instead, if you would like to:

- build the libraries for an unsupported platform
- get the latest version of the code from the GeoModel repository
- develop the libraries

then, you have to **compile them** from scratch. In that case, please follow the instructions provided in the [For Developers](../dev/index.md) section.

If you have problems, please send a message to the mailing list provided in the [Contacts](../contacts.md) page.


## Install the GeoModel visualizer and all the libraries

By installing GeoModelExplorer, the GeoModel visualization tool, you will also install all the GeoModel libraries.

You'll have all the libraries needed to create, store, restore, visualize detector description data.

## macOS

On macOS, we use the Homebrew package manager to handle and install the GeoModel packages. If you don't have Homebrew installed on your machine, you should install it by following the instructions on the Homebrew website: <http://brew.sh>.

You can install the visualization tool and all the libraries by running the following commands:

```
brew tap atlas/geomodel https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel.git
brew install geomodelexplorer
```

The first command will set the Homebrew repository storing the "recipes" to install the ATLAS GeoModel libraries on macOS. The second command install all the libraries.

Pre-compiled packages ("Bottles", in the Homebrew jargon) are provided for the latest macOS release. That speeds up the installation a lot.
For older macOS releases, Homebrew will try to compile the packages from source. If that fails, you will have to compile from source by yourself by following the instructions provided in the [dev](/dev/index.md) section.

## Ubuntu

_Coming soon_

## Centos7 / CC7

_Coming soon_

## SLC6

_Coming soon_
