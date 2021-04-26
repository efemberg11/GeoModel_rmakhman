mkdir -p SampleData
find . -name "*.xml" -exec cp {} SampleData \;
find . -name "*.dtd" -exec cp {} SampleData \;
export GEOMODEL_XML_DIR=`pwd`/SampleData
