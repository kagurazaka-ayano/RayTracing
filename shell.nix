with import <nixpkgs> {}; let
in
  mkShell {
    buildInputs = [
      valgrind

      cmake
      gcc
      gnumake
      ninja
      pkg-config

      clang-tools

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
      ninja
    ];
    # wayland
    NIX_LD_LIBRARY_PATH = lib.makeLibraryPath [stdenv.cc.cc];
    NIX_LD = lib.fileContents "${stdenv.cc}/nix-support/dynamic-linker";
  }
