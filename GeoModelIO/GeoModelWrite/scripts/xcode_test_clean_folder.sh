# A script to be used as "Run Script" in Xcode.
# This is specific to test the target GeoModelWrite with the
# GeoModelExamples/HelloGeoWrite example.
# The script removes the test file geometry.db before running the example.
#
# To use the script, you should add a "Run Script" entry in the target's
# "Build Phases" (the executable, i.e. the test, not the library)
# and add this single line, which will run this script:
#
# $SRCROOT/GeoModelWrite/xcode_test_clean_folder.sh
#
# See: https://www.mokacoding.com/blog/better-build-phase-scripts/
#
#
set -x
ls $TARGET_BUILD_DIR
rm -f $TARGET_BUILD_DIR/geometry.db
