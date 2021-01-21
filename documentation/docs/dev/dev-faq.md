
# Developers' Frequently Asked Questions (FAQ)


## General

### C++ standard and compilers' support

You need **C++17** support to build GeoModel. 

On Linux, you should not have any issue related to that if you use a quite recent distribution.

On macOS, however, there is no proper C++17 support in the native compiler `clang` until Mojave (10.14) and the related Xcode. Thus, if you are running an older macOS release, you only have two options:

- You update your system to a more recent macOS release

- You install GCC on your system and you compile GeoModel with that compiler instead of the native compiler `clang`. This will let you keep your system as it is, without the need of upgrading the main operating system. *(If you choose this second option, we assume you are familiar enough with compilers and development in general, so you don't need step-by-step instructions to compile GeoModel with GCC.)*

## Visualization

### macOS -- Why Do I see everything in 2D / flattened ???

That's what we call the *"Flatland Bug"*, we fear... We have discovered that, on macOS, some combinations of `clang` compiler's and Xcode's versions make the 3D engine we use, Coin, fail somehow.

That only happens on macOS and you get that when you compile the `GeoModelVisualization`'s graphics dependencies, `Coin` and `SoQt` on the affected platforms. The bug affects macOS *Catalina* (not in the 100% of the cases, still investigating why) and *Big Sur* (100%).

We are still investigating this.

There is no fix for it yet, but there is a workaround. For the moment, we would suggest to

What you must do to go around it is:

1. install `gmex` (*i.e.*, the `GeoModelVisualization` executable) from the Homebrew packages (as explained [here](../../start/install/#macos) );  gmex on brew was built from Mojave and it is not affected by Flatland.

        brew install geomodelvisualization

2. clone `GeoModel` and build everything **but** `GeoModelVisualization` locally. You may wish to develop code under `GeoModel` but rarely as an end user you want to develop anything under `GeoModelVisualization`. (As a quick reminder: to exclude `GeoModelVisualization` from the compilation, you can simply omit the CMake `GEOMODEL_BUILD_VISUALIZATION` option from the build configuration) 

3. Code that is client of `GeoModel` (as, for example, `GeoModelATLAS`) is totally unaffected by the *Flatland Bug* and what you built until now should be fine as it is



