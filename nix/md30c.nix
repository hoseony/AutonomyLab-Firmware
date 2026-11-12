{
  stdenv,
  gcc-arm-embedded,
  cmake,
  ninja,
  pico-sdk,
  python3,
  picotool,
  lib,
}:
stdenv.mkDerivation {
  src = lib.fileset.toSource {
    root = ../.;
    fileset = lib.fileset.unions [
      ../CMakeLists.txt
      ../cmake
      ../drivers
      ../examples
    ];
  };

  name = "md30c_driver";

  nativeBuildInputs = [
    cmake
    ninja
    picotool
    gcc-arm-embedded
    python3
  ];

  cmakeFlags = [
    (lib.cmakeFeature "PICO_BOARD" "pico2")
    (lib.cmakeFeature "PICO_SDK_PATH" "${pico-sdk}/lib/pico-sdk")
    (lib.cmakeFeature "CMAKE_C_COMPILER" "${gcc-arm-embedded}/bin/arm-none-eabi-gcc")
    (lib.cmakeFeature "CMAKE_CXX_COMPILER" "${gcc-arm-embedded}/bin/arm-none-eabi-g++")
    (lib.cmakeFeature "picotool_DIR" "${picotool}/lib/cmake/picotool")
  ];

  ninjaFlags = [ "ex_md30c" ];

  installPhase = ''
    runHook preInstall
    install -Dm644 -t $out examples/md30c/ex_md30c.uf2 examples/md30c/ex_md30c.elf
    runHook postInstall
  '';

  dontFixup = true;
}
