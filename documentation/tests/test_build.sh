#!/usr/bin/env bash

# Enforce sourcing of script
[[ "${BASH_SOURCE[0]}" = "${0}" ]] && \
printf "\n# ${BASH_SOURCE[0]} needs to be sourced.\n# Please run again as:\n\n. ./${BASH_SOURCE[0]}\n\n" && \
exit 1

# Note that the image is Alpine based (https://alpinelinux.org/)
docker run \
    --rm \
    -v $PWD:/build \
    -w /build \
    gitlab-registry.cern.ch/authoring/documentation/mkdocs:stable \
    /bin/ash -c "mkdocs build --clean --site-dir _build"

open "${PWD}"/_build/index.html
