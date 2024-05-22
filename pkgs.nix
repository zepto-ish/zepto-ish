{ appendOverlays }:

let
  pkgs = appendOverlays [(final: super: {
    zepto-ish = final.callPackage (
      { stdenv
      , linenoise
      }:
      stdenv.mkDerivation {
        pname = "zepto-ish";
        version = "WIP";
        src = builtins.fetchGit ./.;
        buildInputs = [
          linenoise
        ];
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
