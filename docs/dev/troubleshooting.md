
# Troubleshooting

## Packages

### macOS / Homebrew

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
