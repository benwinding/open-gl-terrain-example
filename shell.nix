{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    clang
    gnumake
    pkg-config
    glfw
    glew
    glm
  ] ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
    mesa
  ];
}
