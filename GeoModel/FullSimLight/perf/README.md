# Performance Profiling of FullSimLight

This directory contains scripts to profile FullSimLight with `perf` and produce
reports of current performance and differences relative to the previous run when
run multiple times in succession. The scripts can be run as part of the
FullSimLight build itself, or as a standalone project, using an existing
installation of FullSimLight.

## System Configuration Required for perf

Most Linux systems have profiling capabilities disabled by default for regular
users due to the security implications it can have. Therefore, in order to be
able to run `perf` as a regular user, you may need to make some adjustments to
the kernel configuration of your machine. The most important configuration is
`kernel.perf_event_paranoid`, which should be set to a value less than 1,
preferrably to -1. Below is an exerpt showing how to check and set this
parameter using `sysctl`:

```sh
$ sysctl kernel.perf_event_paranoid
kernel.perf_event_paranoid = 2
$ sudo sysctl kernel.perf_event_paranoid=-1
kernel.perf_event_paranoid = -1
```

If you want this change to persist across reboots, you can add it to the system
configuration file at `/etc/sysctl.conf`. Please see the man page for
`sysctl(8)` for more information. Similarly, two other configuration knobs are
important, especially when using Intel VTune along with `perf`. The first one is
`kernel.nmi_watchdog`, which should be disabled (set to 0), and the other one is
`kernel.kptr_restrict`, which should also be disabled. Certain data collection
types in VTune will only work if the NMI watchdog is disabled in the kernel, and
profiling that includes the kernel requires its address map to be unrestricted
via the second configuration knob above. You may need to configure your kernel
to be compiled with debugging information in addition to this, or install the
`kernel-debug` package for the version of your running kernel.

The last part of the configuration is not a requirement, but will help ensure
measurements are more stable. It is recommended that both hyperthreading and
turbo boost be disabled if possible. Hyperthreading is usually disabled in the
BIOS settings, whereas for turbo boost it may depend on your particular machine.
For AMD machines it is also usually in the BIOS, but on Intel machines it can
usually be disabled with a simple kernel configuration as well. To check if it
is enabled, check the contents of `/sys/devices/system/cpu/intel_pstate/no_turbo`.
If it is set to zero, you can set it to 1 to disable turbo boost like shown below

```sh
$ echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo
$ cat /sys/devices/system/cpu/intel_pstate/no_turbo
1
```

Note that you need to be the `root` user to do this, and that after a reboot the
value will be reset to its default value. Some machines also allow to force the
CPU into running at a fixed frequency, which is a good idea. If not, try to set
the CPU governor to `performance` using a tool like `cpupower` or `ncpufreqd`.

## Configuring a FullSimLight Build for Profiling

In order to collect accurate information when profiling FullSimLight, it is
important to use an appropriately configured build. Since perf relies on the
binaries to extract symbol information, it is important to enable debugging
information by using a `RelWithDebInfo` CMake build type, or by adding `-g` to
the compilation flags. Using DWARF debugging information allows one to use
`perf`'s more accurate `dwarf` mode for stack unwinding when recording call
graph information with `perf`. This mode produces much larger data files, but
has the advantage that is allows `perf` to attribute lines of code from inlined
functions to themselves rather than the calling functions, for example.

Another important compile flag to enable is `-fno-omit-frame-pointer`. This
option prevents the compiler from disabling the stack frame pointer. The stack
frame pointer is needed by `perf`'s `fp` mode of stack unwinding, which is the
second best choice after `dwarf`. The last mode `lbr` is not supported by all
CPU vendors and cannot handle deep stack frames, so using one of `fp` or `dwarf`
modes is strongly recommended. The scripts in this directory uses the default
configuration from `perf` (usually `fp`). In order to use a different mode, one
can call

```sh
$ perf config call-graph.record-mode=<mode>
```

to change the default mode.

### Using `RPATH` instead of `LD_LIBRARY_PATH`

Although very frequently used, the `LD_LIBRARY_PATH` environment variable
should be avoided whenever possible. When profiling, we are often comparing
different versions of the same library (e.g. Geant4), and `LD_LIBRARY_PATH`
makes it easy to run with the wrong version of the library by requiring the
environment to be cleaned up and reconfigured frequently. Other reasons why
using `LD_LIBRARY_PATH` is bad are discussed [here][1]. Using `RPATH` is a
much better option to ensure the correct versions of all libraries are always
used, as they are fixed at build time.

CMake provides options to control the `RPATH` setting when building, and often
simply adding `-DCMAKE_SKIP_INSTALL_RPATH=FALSE` is enough to avoid having to
set `LD_LIBRARY_PATH` after installation. However, the best way to enable
`RPATH` is to use a relative one, so that relocatable packages continue to work
after being moved. The linker understands the special variables `$ORIGIN` and
`$LIB` that resolve to the directory of the binary (executable or library) and
the name of the default library directory (e.g. `lib` or `lib64`). Therefore,
adding `-DCMAKE_INSTALL_RPATH='$ORIGIN/../$LIB'` to the CMake configuration will
configure the project to use a relative `RPATH` that works most of the time for
both binaries and libraries. Some projects hard-code the name of the library
directory (i.e. always use `lib` instead of `lib64` when appropriate), so
adjustments to the above may be necessary. Geant4 only builds libraries, so using
`-DCMAKE_INSTALL_RPATH='$ORIGIN'` is enough to work regardless of what the
library directory is. When installing FullSimLight, we recommend using
`'$ORIGIN/../$LIB'` for the `RPATH` as shown above.

1. http://xahlee.info/UnixResource\_dir/\_/ldpath.html

## Profiling FullSimLight using this Project

This project tries to detect most things and use sensible defaults as much as
possible, but most parameters can also be easily configured via CMake variables.

The default profiling job is to run FullSimLight with the builtin Pythia input
configuration for ttbar events at E = 14 TeV, as we want to profile something
as realistic as possible by default. Therefore, Pythia needs to be enabled when
building FullSimLight or the default job needs to be changed by hand by editing
the default test command in the `CMakeLists.txt`. Most other options can be set
via CMake.

The binary for FullSimLight can be set with
`-DFULLSIMLIGHT=/path/to/fullSimLight`. This option must be set every time when
in use. The default is to always search for the binary so that reconfigurations
can change the version that was stored in the CMake cache.

When using specific versions of Geant4 and this project as external to
FullSimLight, please set `-DGeant4_DIR` or `-DCMAKE_PREFIX_PATH` to Geant4's
install directory so that the environment variables for the same Geant4 version
are used (this project sets them for you automatically using `geant4-config`).
To check if the right datasets are being used, check the output of `ctest -VV`.
It will show which environment variables have been set for you.

The number of cores to use when profiling will be detected automatically, but
can be specified with `-DNCORES=N` as well.

The number of events can be controlled with either `-DEVENTS_PER_CORE` or with
`-DEVENTS` options. The default is to use 4 events per core. This option adds
the appropriate `/run/BeamOn` command into the default macro file and will be
ignored if you decide to use a different macro.

A custom macro can be used with `-DMACRO=/path/to/macro.g4` and will replace
the default one with detected number of cores and events. The default macro
will set a constant magnetic field of 4 Tesla if the field map file is not
found in the build directory. To use the field map, just drop the file into
the build directory and reconfigure. A blank line should appear in the default
macro to allow FullSimLight to load the field map by default instead.

The geometry file can be set with `-DGEOMETRY`. By default, it will use the
`geometry-ATLAS-R2-2016-01-02-01_with_EMEC_Wheel.db`, which is automatically
downloaded from https://gitlab.cern.ch/GeoModelATLAS/geometry-data if not found.
Any geometry filename from this repository can be set and will be downloaded
automatically. When setting to a local file, just use the full path to a GDML
or SQLite file.

Default metrics to collect with `perf` are defined in `CMakeLists.txt`, but
can be customized. However, for the report to work, the default values must
be included in the custom list of events as well, as the scripts expect those
events in the resulting data files.

After configuring the project, a report can be created simply by running `ctest`.
For running only the collection of profiling data, use `ctest -L record`, and
to run only the report part, `ctest -L report`. To run both recording and
reporting parts together, use `ctest -L perf`. The main report is written to
`fullsimlight.txt` and the flamegraph to `fullsimlight.svg`.

### Profiling FullSimLight by itself

If you are only interested in the results of a single report, you can just
enable the option `-DGEOMODEL_BUILD_FULLSIMLIGHT_PROFILING=ON`, and then run
`ctest -L perf` after configuring and building the project. This is the simplest
way of getting a performance report.

### Profiling FullSimLight against different versions of Geant4

Profiling the same version of FullSimLight against different versions of Geant4
is also straitghtforward and can be done using the integrated method as above.
The only difference is that after recording profiling information for the first
time, you need to rebuild FullSimLight using another Geant4 version and then
rerun the tests. For example, suppose that we installed two versions of Geant4
into `/opt/geant4/10.6` and `/opt/geant4/10.7` and want to compare how they
perform with FullSimLight. This can be done with:

```sh
GeoModel $ mkdir build && cd build
build $ cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_PREFIX_PATH=/opt/geant4/10.6 -DDGEOMODEL_BUILD_FULLSIMLIGHT_PROFILING=ON ...
build $ cmake --build .
build $ ctest -L perf
```

then

```sh
build $ cmake . -DCMAKE_PREFIX_PATH=/opt/geant4/10.7
build $ cmake --build .
build $ ctest -L perf
```

The second report should automatically show the differences with the first one.

Something to keep in mind when comparing different versions is that you should
not overwrite any libraries used by the first version. This is because `perf`
uses each library to resolve the symbols on demand, and overwriting the library
will make it show wrong symbols for the run before the library has been
overwritten. For this reason, when comparing two versions of FullSimLight
itself, it's not enough to simply rebuild and run the tests again, as that will
overwrite the current build's libraries and confuse `perf`.

### Profiling different versions of FullSimLight itself

Comparing different versions of FullSimLight requires some care, as explained
above. If the two versions differ only in configuration, then it is ok to use
the integrated build and just rerun the tests for the different configurations.
However, if a change has been made to FullSimLight itself, then both versions
must be kept so that `perf` can properly resolve symbols from each version at
the same time. In this case, it's best to install each FullSimLight version
somewhere with

```sh
GeoModel $ mkdir build && cd build
build $ cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_INSTALL_PREFIX=/opt/fullsimlight/old ...
build $ cmake --build .
build $ cmake --build . --target install
```

Make changes (or switch branches) in FullSimLight and then rebuild:

```sh
build $ cmake . -DCMAKE_INSTALL_PREFIX=/opt/fullsimlight/new
build $ cmake --build .
build $ cmake --build . --target install

```

Now that both versions are installed, use this CMake project to just run the
profiling for each installed version:

```sh
GeoModel $ cd FullSimLight/perf
perf $ mkdir build && cd build
build $ cmake .. -DCMAKE_PREFIX_PATH=/opt/fullsimlight/old
build $ ctest
build $ cmake .. -DCMAKE_PREFIX_PATH=/opt/fullsimlight/new
build $ ctest
```

