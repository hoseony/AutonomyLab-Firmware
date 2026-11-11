{
  stdenv,
  fetchFromGitHub,
  cmake,
  lib,
  pico-sdk,
  picotool,
  gcc-arm-embedded,
  python3,
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "debugprobe_on_pico2";
  version = "2.3.1";

  src = fetchFromGitHub {
    owner = "raspberrypi";
    repo = "debugprobe";
    rev = "debugprobe-v${finalAttrs.version}";
    hash = "sha256-o3hARsvUrpnSjyugeGq6ECEnHrbm/xC6EvswSsgPoL4=";
    fetchSubmodules = true;
  };

  nativeBuildInputs = [
    cmake
    gcc-arm-embedded
    picotool
    python3
  ];

  cmakeFlags = [
    (lib.cmakeFeature "PICO_BOARD" "pico2")
    (lib.cmakeFeature "PICO_SDK_PATH" "${pico-sdk}/lib/pico-sdk")
    (lib.cmakeFeature "CMAKE_C_COMPILER" "${gcc-arm-embedded}/bin/arm-none-eabi-gcc")
    (lib.cmakeFeature "CMAKE_CXX_COMPILER" "${gcc-arm-embedded}/bin/arm-none-eabi-g++")
    (lib.cmakeFeature "picotool_DIR" "${picotool}/lib/cmake/picotool")
  ];

  installPhase = ''
    runHook preInstall
    install -Dm644 -t $out debugprobe*.uf2 debugprobe*.elf
    runHook postInstall
  '';

  dontFixup = true;
})
