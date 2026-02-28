#!/usr/bin/env bash
set -euo pipefail

if command -v nix >/dev/null 2>&1; then
  if nix --version 2>/dev/null | grep -q "flake"; then
    nix --extra-experimental-features "nix-command flakes" develop --command "make -B web"
  else
    nix-shell --command "make web"
  fi
else
  echo "nix is not installed. See https://nixos.org/download/" >&2
  exit 1
fi
