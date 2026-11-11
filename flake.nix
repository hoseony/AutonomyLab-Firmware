{
  description = "Autonomy lab environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    inputs:
    let
      # Helper list??
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "aarch64-darwin"
      ];
      # Helper function
      forAllSystems =
        f:
        map (system: {
          name = system;
          value = f system;
        }) systems;
    in
    {

      packages = builtins.listToAttrs (
        forAllSystems (
          system:
          let
            pkgs = inputs.nixpkgs.legacyPackages.${system};
            pico-sdk = pkgs.pico-sdk.override { withSubmodules = true; };
          in
          {
            debugprobe = pkgs.callPackage ./nix/debugprobe.nix { inherit pico-sdk; };
          }
        )
      );

      # Basic dev shells, includes the packages we need for development
      devShells = builtins.listToAttrs (
        forAllSystems (
          system:
          let
            pkgs = inputs.nixpkgs.legacyPackages.${system};
            pico-sdk = pkgs.pico-sdk.override { withSubmodules = true; };
            openocd-rp2350 = pkgs.openocd-rp2040.overrideAttrs (
              finalAttrs: old: {
                version = "2.3.0";
                src = pkgs.fetchFromGitHub {
                  owner = "raspberrypi";
                  repo = "openocd";
                  tag = "sdk-${finalAttrs.version}";
                  hash = "sha256-1fS+cyPvh+KQUqCx5d3/oCOMXORvXoHSmx9p0ZW9uE0=";
                  fetchSubmodules = false;
                };
              }
            );
          in
          {
            default = pkgs.mkShell {
              packages = with pkgs; [
                cmake
                pkg-config
                python3

                gcc-arm-embedded
                gdb

                picotool
                gnumake
                pico-sdk
                openocd-rp2350

                ninja
                mdbook
                clang-tools
                minicom

              ];
              env = {
                PICO_SDK_PATH = "${pico-sdk}/lib/pico-sdk";
              };
            };
          }
        )
      );
    };
}
