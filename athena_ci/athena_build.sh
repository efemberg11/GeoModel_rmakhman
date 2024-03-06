#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}"  )" &> /dev/null && pwd  )

function run() { 
    set -x
    "$@" 
    { set +x;  } 2> /dev/null
}

columns=80
if [ -t 0  ]; then
    columns="$(tput cols)"
fi

function fill_line() {
    sep=${1:-=}
    printf "=%.0s" $(seq 1 "$columns") | tr "=" "$sep"
    echo ""
}
 
function print_center() {
    text="$1"
    printf "%*s\n" $(((${#text}+$columns)/2)) "$text"
}

function heading() {
    fill_line
    print_center "$1"
    fill_line
}



set -e

heading "Preliminary setup"

if [ -z "${ATHENA_SOURCE}" ]; then
    ATHENA_SOURCE=$PWD/athena
fi

echo "ATHENA_SOURCE: ${ATHENA_SOURCE}"

source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh || true
asetup none,gcc11 || true
lsetup cmake || true
lsetup git || true
NINJA=/cvmfs/sft.cern.ch/lcg/releases/ninja/1.10.0-d608d/x86_64-el9-gcc13-opt/bin/ninja

EXTRA_FLAGS=""
if [ -t 1  ]; then
    EXTRA_FLAGS="-fdiagnostics-color=always"
fi

heading "Athena setup"

if [[ ! -d "${ATHENA_SOURCE}"  ]]; then
    echo "Cloning Athena: ${ATHENA_GIT_REPO} @ ${ATHENA_REF}"
    git clone ${ATHENA_GIT_REPO} -b ${ATHENA_REF} ${ATHENA_SOURCE}
    pushd "${ATHENA_SOURCE}"
    $SCRIPT_DIR/apply_patches.sh
    popd

else 
    read -p "${ATHENA_SOURCE} already exists; wipe the repository or keep it (wipe/keep)? [keep] " CONT
    if [ -z "$CONT" ]; then
        CONT="keep"
    fi
    echo $CONT
    if [ "$CONT" = "wipe" ]; then
        echo "Reseting Athena in ${ATHENA_SOURCE} to expected state";

        pushd "${ATHENA_SOURCE}"
        run git reset --hard HEAD
        run git checkout ${ATHENA_REF}
        run git pull
        $SCRIPT_DIR/apply_patches.sh
        popd
    else
        echo "Keeping Athena checkout as is"
    fi
fi

LCG_VERSION_NUMBER=$(sed -n "s/.*LCG_VERSION_NUMBER=\(\S*\)$/\1/p" ${ATHENA_SOURCE}/Projects/Athena/build_externals.sh)
LCG_VERSION_POSTFIX=$(sed -n "s/.*LCG_VERSION_POSTFIX=\"\(\S*\)\"$/\1/p" ${ATHENA_SOURCE}/Projects/Athena/build_externals.sh)
LCG_RELEASE="LCG_${LCG_VERSION_NUMBER}${LCG_VERSION_POSTFIX}"

echo "LCG_RELEASE: ${LCG_RELEASE}"
echo "LCG_PLATFORM: ${LCG_PLATFORM}"

lsetup "views ${LCG_RELEASE} ${LCG_PLATFORM}" || true

CCACHE=$(command -v ccache)
$CCACHE -z

heading "Configure GeoModel"

gm_install_dir=$PWD/geomodel-install

cmake -S "${SCRIPT_DIR}/.." -B geomodel-build \
  -GNinja \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_CXX_FLAGS="$EXTRA_FLAGS" \
  -DCMAKE_INSTALL_PREFIX=$gm_install_dir \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DGEOMODEL_BUILD_TOOLS=ON \
  -DCMAKE_CXX_COMPILER_LAUNCHER=$CCACHE \

heading "Build GeoModel"

cmake --build geomodel-build
$CCACHE -s

echo "Installing GeoModel"
cmake --install geomodel-build > gm_install.log

heading "Setup Athena"

asetup Athena,${ATHENA_RELEASE},latest || true


n=10
fill_line "="
pushd ${ATHENA_SOURCE} > /dev/null
echo
echo "Last $n commits:"
fill_line "-"
git --no-pager log -n$n --pretty=short
fill_line "-"
popd > /dev/null
echo
fill_line "="

$CCACHE -z

export CMAKE_PREFIX_PATH="${gm_install_dir}:$CMAKE_PREFIX_PATH"


heading "Configure Athena"

full_package_filters=$SCRIPT_DIR/package_filters.txt
patch_package_filters=$SCRIPT_DIR/patch_package_filters.txt

package_filters=$(mktemp)


echo "IS_MERGE_REQUEST = $IS_MERGE_REQUEST"
echo "HEADERS_CHANGED = $HEADERS_CHANGED"

if [ "$IS_MERGE_REQUEST" = "0" ] && [ "$HEADERS_CHANGED" = "1" ]; then
    echo "Is MR and header files have NOT changed"
else
    echo "Is NOT MR OR header files HAVE changed"
    cat "$full_package_filters" > "$package_filters"
fi

cat "$patch_package_filters" >> "$package_filters"

echo "- .*" >> "$package_filters"

echo "$package_filters"
fill_line "-"
cat "$package_filters"
fill_line "-"

mkdir athena-build
install_dir=$PWD/athena-install
pushd athena-build
cmake "$ATHENA_SOURCE/Projects/WorkDir" \
  -GNinja \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -DCMAKE_CXX_FLAGS="$EXTRA_FLAGS" \
	-DATLAS_PACKAGE_FILTER_FILE="$package_filters" \
  -DCMAKE_CXX_COMPILER_LAUNCHER=$CCACHE \
  -DCMAKE_INSTALL_PREFIX=$install_dir
popd

echo "Patching env_setup.sh to pick up correct GeoModel shared libs"

echo "" >> athena-build/*/env_setup.sh
echo "# GeoModel hack build injected library path:" >> athena-build/*/env_setup.sh
echo "export LD_LIBRARY_PATH=\"${gm_install_dir}/lib64:\$LD_LIBRARY_PATH\"" >> athena-build/*/env_setup.sh

heading "Build Athena"

if [ -z "$CI" ]; then
  heading "Interactive mode, dropping into shell"
  bash
else
  cmake --build athena-build -- -j3
  $CCACHE -s
  cmake --install athena-build > athena_install.log
fi
