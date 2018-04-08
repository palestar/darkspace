#!/usr/bin/env bash

set -euo pipefail

medusa_builds=( "Medusa" "Network" "GCQ" "Render3D" "World" )
darkspace_builds=( "DarkSpace" "Gadgets" "DarkSpaceServer" )
gamecq_builds=( "ChronDemon" "GCQB" "GCQDB" "GCQS" "LogServer" "MetaServer" "MirrorServer" "ProcessServer" "RotateLogs" "ProcessClientCLI" )

build() {
  local directory="${1}"
  local builds="$2"

  pushd "${directory}"

  for build in "${builds[@]}"; do
    echo "Building ${directory}/${build}..."
    pushd "${build}"
    make -j2 BUILD_CONFIGURATION="ReleaseLinux" CC=$CC CXX=$CXX INTERMEDIATE_DIR="./ReleaseLinux/obj" DESTINATION_DIR="../Bin"
    popd
  done

  popd
}

main() {
  pushd "${WORK}"
  build Medusa "${medusa_builds[@]}"
  build darkspace "${darkspace_builds[@]}"
  build GameCQ "${gamecq_builds[@]}"
}

main "$@"
