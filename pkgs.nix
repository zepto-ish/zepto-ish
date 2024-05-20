{ appendOverlays }:

let
  pkgs = appendOverlays [(final: super: {
    zepto-ish = final.callPackage (
      { stdenv
      }:
      stdenv.mkDerivation {
        pname = "zepto-ish";
        version = "WIP";
        src = builtins.fetchGit ./.;
        makeFlags = [
          "PREFIX=${placeholder "out"}"
        ];
      }
    ) {};
  })];
in
{
  inherit (pkgs) zepto-ish;
  cross = {
    aarch64 = pkgs.pkgsCross.aarch64-multiplatform.zepto-ish;
  };
}
