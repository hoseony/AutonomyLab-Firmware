{
  stdenvNoCC,
  lib,
  fetchFromGitHub,
}:

stdenvNoCC.mkDerivation {

  pname = "freertos-kernel-rpi";
  version = "unstable-2024-08-08";

  src = fetchFromGitHub {
    owner = "raspberrypi";
    repo = "FreeRTOS-Kernel";
    rev = "4f7299d6ea746b27a9dd19e87af568e34bd65b15"; # Current latest revision
    hash = "sha256-rZmoq24aNmnDr3t1qk1n88hj5mMz5vDrMsGwgVX4FXQ=";
  };

  dontConfigure = true;
  dontBuild = true;

  installPhase = ''
    runHook preInstall
    cp -r . $out
    runHook postInstall
  '';

  meta = {
    description = "FreeRTOS kernel fork with RP2040/RP2350 ports";
    homepage = "https://github.com/raspberrypi/FreeRTOS-Kernel";
    license = lib.licenses.mit;
  };
}
