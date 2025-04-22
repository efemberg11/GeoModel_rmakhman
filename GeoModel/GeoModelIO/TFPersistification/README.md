# GeoModelIO::TFPersistification build

## Build external dependencies

### Eigen

```bash
brew install eigen
```


## Build GeoModelIO::TFPersistification

```bash
git clone
mkdir TFPersistification_build
cd TFPersistification_build
cmake ../TFPersistification -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=../install
make -j10
make install
```
