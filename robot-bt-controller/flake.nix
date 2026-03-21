{
  description = "Go Development Shell with SDL2";

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
              pkgs.pkg-config
              pkgs.SDL2
              pkgs.libX11
              pkgs.zsh
            ];

            shellHook = ''
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
