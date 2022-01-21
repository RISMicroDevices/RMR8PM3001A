#!/bin/bash

VERSION="0.12 (RISMD)"
DESCRIPTION="MEMU (Mixed EMUlation) verification batch"

help() {
    echo $DESCRIPTION
    echo "Version v"$VERSION
    echo "Usage: "
    echo "build-test-memu.sh [-e case_name] [-p memu_root] [-a parameter_list]"
    echo "Description:"
    echo "-c: Specify the compiler. \"g++\" by default."
    echo "-e: Specify the verification case name. For example: -e dut."
    echo "-p: Specify the mixed emulation components root directory."
    echo "-a: Parameters passed to the verification program. For example: -a \"1 2 3 ......\"."
    echo "-o: Specify the working directory."
    exit 0
}

# Initialize parameters
WORKSPACE_PATH=$(dirname $(readlink -f "$0"))
BUILD_FOLDER="build_test"
EMU_FILE="emu"

COMPILER="g++"

CASE_FOLDER="dut_example"
MEMU_FOLDER=
PARAMETERS=
INCLUDE_FOLDERS=

# Check input parameters
while getopts 'hc:e:p:a:o:' OPT; do
    case $OPT in
        h) help;;
        c) COMPILER="$OPTARG";;
        e) CASE_FOLDER="$OPTARG";;
        p) MEMU_FOLDER="$OPTARG";;
        a) PARAMETERS="$OPTARG";;
        o) WORKSPACE_PATH="$OPTARG";;
        ?) help;;
    esac
done

# Enter case work path
if [[ -n "$CASE_FOLDER" ]]
then
    CASE_PATH="$WORKSPACE_PATH/$CASE_FOLDER"
    BUILD_PATH="$CASE_PATH/$BUILD_FOLDER"
    BUILD_FILE="$BUILD_PATH/$EMU_FILE"

    cd $CASE_PATH

    if [ $? -ne 0 ]; then
        exit 1
    fi
else
    echo "More parameters required. Please specify the case root by \"-e\"."
    echo "Use \"-h\" to get more detailed information."
    exit 1
fi

# Include all mixed elaboration header files
if [[ -n "$MEMU_FOLDER" ]]
then
    INCLUDE_FOLDERS="$INCLUDE_FOLDERS -I$MEMU_FOLDER"
else
    echo "More parameters required. Please specify the MEMU components root by \"-p\"."
    echo "Use \"-h\" to get more detailed information."
    exit 1
fi

# Build and run
eval "mkdir $BUILD_PATH"
echo "Ready to compile MEMU verification payload."

echo "[compile task] \"$COMPILER main.cpp $INCLUDE_FOLDERS -o $BUILD_FILE\""
eval "$COMPILER main.cpp $INCLUDE_FOLDERS -o $BUILD_FILE"
if [ $? -ne 0 ]; then
    echo "Failed at compilation."
    exit 1
fi
echo "Payload compiled."

echo "[run task] $BUILD_FILE"
cd $BUILD_PATH
echo "--------------------------------"
eval "./$EMU_FILE"
echo "--------------------------------"
echo "Payload finished with return value $?."
