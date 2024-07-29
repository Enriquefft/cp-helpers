{
  description = "A basic flakelight templ to be used with nix-direnv";

  inputs = {

    flakelight.url = "github:nix-community/flakelight";

  };

  outputs = { flakelight, nixpkgs, ... }:
    flakelight ./. {
      inputs.nixpkgs = nixpkgs;
      devShell = pkgs: {

        stdenv = pkgs.llvmPackages_latest.stdenv;

        packages = with pkgs;
          [
            coreutils
            clang-tools
            cmake
            cppcheck
            doxygen
            gtest
            lcov
            vcpkg
            vcpkg-tool
            gdb

            python3

          ] ++ (with pkgs.python3Packages;
            [

            ]);
      };
    };

}
