with import <nixpkgs> {}; let
in
  clangStdenv.mkDerivation {
    name = "rt";
    buildInputs = [
      gcc
      clang_multi
      vulkan-tools
      vulkan-loader
      pkg-config
      xorg.libX11
      xorg.libXrandr
      xorg.libXinerama
      xorg.libXcursor
      xorg.libXi
      vulkan-headers
      cmake
    ];
    # wayland
    NIX_LD_LIBRARY_PATH = lib.makeLibraryPath [stdenv.cc.cc cmake ninja];
    NIX_LD = lib.fileContents "${stdenv.cc}/nix-support/dynamic-linker";
  }
