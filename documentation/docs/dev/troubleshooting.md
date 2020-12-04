
# Troubleshooting

## Packages

### macOS / Homebrew

#### Cleaning your environment

**PACKAGES**

Sometimes, cleaning the system from old packages helps to solve `brew`-related issues.

Also, please clean your system from old packages before installing new packages, or new versions of the packages. This will fix installation issues, in case `brew` does not succeed to correctly upgrade the installed packages.

You can see the list of all `brew` packages installed on your system with: 

```
brew list
```

Then, you can clean the system by removing the GeoModel packages. 
To do so, just type `brew remove` and the name of the packages that need to be removed (you might be asked to use `brew uninstall` for specific cases; in that case, you will get a clear error message from `brew` about that and you should merely follow the on-screen instructions).

If you see packages like `geomodelcore` in the output of `brew list`, that means that you have old versions of the GeoModel packages installed. Please remove them and update to the latest packages; for example, by using (please remove from the commands below the packages that are not present on your system, otherwise `brew` will complain):

```bash
brew remove geomodelexplorer geomodelio geomodelcore geomodelvisualization
brew remove soqt-bb coin-bb simage
```

Also, if you see packages like `geomodel`, please remove them before trying to compile GeoModel from sources.
You can clean your system from recent versions of the packages by running:

```
brew remove geomodel geomodelvisualization
```

You can also remove the graphical libraries by running:

```
brew remove coin-geomodel soqt-geomodel simage-geomodel
```


**TAPS**

*Taps* are remote repositories used to store `brew` formulas. GeoModel has a Tap called `atlas/geomodel`, which is stored on the CERN GitLab server:
https://gitlab.cern.ch/GeoModelDev/packaging/homebrew-geomodel

In order to have a clean environment, you should remove old GeoModel related Taps.

At first, check the Taps that are installed on your systems; please, type:

```
brew tap
```

You should see at least:

```
atlas/geomodel
homebrew/cask
homebrew/core
homebrew/services
```

plus any other tap you installed on your own for other reasons.

In particular, if you see taps like the ones here below:

```
ric-bianchi/coin3d
ric-bianchi/vp1light
jfb3615/repo
atlas/graphics
```

those are old repositories containing old formulas, which conflict with the latest version of the formulas hosted in the `atlas/geomodel` tap.

Please remove those taps by typing:

```
brew untap ric-bianchi/coin3d  ric-bianchi/vp1light jfb3615/repo atlas/graphics
```


#### Error: SHA256 mismatch

If you see an output similar to the one below:

```bash
==> Installing geomodelio from atlas/geomodel
==> Downloading https://gitlab.cern.ch/GeoModelDev/GeoModelIO/-/archive/3.2.0/GeoModelIO-3.2.0.zip
Already downloaded: /Users/user/Library/Caches/Homebrew/downloads/0cc80cc8d2a1e3066725c5ee623c23c7f329a086ea892fdc7cc6dd5b226694e2--GeoModelIO-3.2.0.zip
==> Verifying 0cc80cc8d2a1e3066725c5ee623c23c7f329a086ea892fdc7cc6dd5b226694e2--GeoModelIO-3.2.0.zip checksum
Error: SHA256 mismatch
Expected: 5dcbedff90f88088af8fc6c94b16f73d1e43d02dbf49ebd2087cff3423ab20bd
  Actual: 050860303807f186a36d7caaf386be5ed0af710d3d44994184b391781a666dc3
 Archive: /Users/user/Library/Caches/Homebrew/downloads/0cc80cc8d2a1e3066725c5ee623c23c7f329a086ea892fdc7cc6dd5b226694e2--GeoModelIO-3.2.0.zip
To retry an incomplete download, remove the file above.
```

It means you had a file downloaded by brew at an earlier stage, and the hash (the *signature*) mismatches. Try to remove the old file from your system, before running the same command again.

In this example, try to remove:

```
rm /Users/user/Library/Caches/Homebrew/downloads/0cc80cc8d2a1e3066725c5ee623c23c7f329a086ea892fdc7cc6dd5b226694e2--GeoModelIO-3.2.0.zip
```

and then retry to install.
