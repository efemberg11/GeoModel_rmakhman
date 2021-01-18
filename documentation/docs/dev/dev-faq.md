
# Developers' Frequently Asked Questions (FAQ)

## C++ standard and compilers' support

You need **C++17** support to build GeoModel. 

On Linux, you should not have any issue related to that if you use a quite recent distribution.

On macOS, however, there is no proper C++17 support in the native compiler `clang` until Mojave (10.14) and the related Xcode. Thus, if you are running an older macOS release, you only have two options:

- You update your system to a more recent macOS release

- You install GCC on your system and you compile GeoModel with that compiler instead of the native compiler `clang`. This will let you keep your system as it is, without the need of upgrading the main operating system. *(If you choose this second option, we assume you are familiar enough with compilers and development in general, so you don't need step-by-step instructions to compile GeoModel with GCC.)*



