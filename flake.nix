{
  description = "Autonomy lab environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    inputs:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "aarch64-darwin"
      ];
      forAllSystems =
        f:
        map (system: {
          name = system;
          value = f system;
        }) systems;
    in
    {
      # Basic dev shells
      devShells = builtins.listToAttrs (
        forAllSystems (
          system:
          let
            pkgs = inputs.nixpkgs.legacyPackages.${system};
            pico-sdk = pkgs.pico-sdk.override { withSubmodules = true; };
          in
          {
            default = pkgs.mkShell {
              packages = with pkgs; [
                cmake
                pkg-config
                python3

                gcc-arm-embedded

                picotool
                openocd
                gnumake
                pico-sdk
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
