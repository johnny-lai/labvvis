#!/bin/bash
PGMAKER="/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker"

echo "Clearing Install"
rm -rf Install/*

echo "Exporting from SVN ... "
#rm -r Staging/Applications/LabVVIS/labvvis
#svn export --force http://localhost:81/svn/thesis/trunk/thesis/projects/labvvis/labvvis Staging/Applications/LabVVIS/labvvis

echo "Building Metapackage ..."
$PGMAKER -build \
	-p "Install/LabVVIS.mpkg" \
	-proj "LabVVIS.pmproj"
mkdir Install/LabVVIS.mpkg/Contents/Packages

echo "Building LabVVIS Frameworks.pkg ..."
$PGMAKER -build \
	-p "Install/LabVVIS.mpkg/Contents/Packages/LabVVIS Frameworks.pkg" \
	-f "Staging/Frameworks" \
	-d "frameworks-Description.plist" \
	-i "frameworks-Info.plist"

#echo "Building LabVVIS lib.pkg ..."
#$PGMAKER -build \
#	-p "Install/LabVVIS.mpkg/Contents/Packages/LabVVIS lib.pkg" \
#	-f "Staging/Frameworks" \
#	-d "lib-Description.plist" \
#	-i "lib-Info.plist"

echo "Exporting examples from SVN ..."
#rm -r Staging/Applications/LabVVIS/examples
#svn export --force http://localhost:81/svn/thesis/trunk/thesis/projects/labvvis/examples Staging/Applications/LabVVIS/Examples

echo "Building LabVVIS Applications.pkg ..."
$PGMAKER  -build \
	-p "Install/LabVVIS.mpkg/Contents/Packages/LabVVIS Applications.pkg" \
	-f "Staging/Applications" \
	-d "apps-Description.plist" \
	-i "apps-Info.plist"

echo "Copying Readme.txt"
cp Readme.txt Install/

echo "Copying uninstall.sh"
cp uninstall.sh Install/

