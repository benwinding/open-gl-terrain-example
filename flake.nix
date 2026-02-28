{
  description = "OpenGL terrain demo dev shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "aarch64-darwin" "x86_64-darwin" "aarch64-linux" "x86_64-linux" ];
      forAllSystems = nixpkgs.lib.genAttrs systems;
    in {
      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in {
          default = pkgs.mkShell {
            packages = with pkgs; [
              clang
              gnumake
              pkg-config
              glfw
              glew
              glm
              emscripten
            ] ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
              mesa
            ];
          };
        });
    };
}
