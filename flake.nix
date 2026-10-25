#from: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

{
	description = "Raylib development environment";

	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
	};

	outputs = {self, nixpkgs, ...}:
	let
		system = "x86_64-linux";
    
		pkgs = import nixpkgs {inherit system;};
	in 
	{
		devShells."${system}".default = pkgs.mkShell {
    
			packages = with pkgs; [
		
				#my additions
				raylib
				gcc
				gdb
				python3
				#------------
		
				libGL

				# X11 dependencies
				libx11
				libx11.dev
				libxcursor
				libxi
				libxinerama
				libxrandr

				#web support
				emscripten
			];

			LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [pkgs.alsa-lib];
		};
	};
}
