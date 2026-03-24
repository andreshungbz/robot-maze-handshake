{
  description = "Go Development Shell with OpenCV";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-darwin"
      ];
    in
    {
      devShells = nixpkgs.lib.genAttrs systems (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.mkShell {
            buildInputs = [
              pkgs.go
              pkgs.gotests
              pkgs.gopls
              pkgs.impl
              pkgs.delve
              pkgs.gotools
              pkgs.gnumake
              pkgs.pkg-config
              pkgs.opencv
              pkgs.zsh
            ];

            shellHook = ''
              export CGO_CFLAGS="-U_FORTIFY_SOURCE"
              export CGO_CPPFLAGS="-U_FORTIFY_SOURCE"
              export CGO_LDFLAGS=""

              if [ -z "$ZSH_VERSION" ]; then
                export SHELL=${pkgs.zsh}/bin/zsh
                exec ${pkgs.zsh}/bin/zsh -l
              fi
            '';
          };
        }
      );
    };
}
