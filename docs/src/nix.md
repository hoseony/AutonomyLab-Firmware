# Nix

We will be using nix as it's a useful tool for managing dependencies and ensuring reproduceability.

To use nix, you have to install it.

Installation:
For MacOS: `curl --proto '=https' --tlsv1.2 -L https://nixos.org/nix/install | sh` (multi-user)
For Linux: `curl --proto '=https' --tlsv1.2 -L https://nixos.org/nix/install | sh -s -- --no-daemon` (single-user)

For other operating systems, refer to the instructions on [Nix Installation](https://nixos.org/download/#nix-install-linux).

Run:
```bash
mkdir -p ~/.config/nix
echo "experimental-features = flakes nix-command" >> ~/.config/nix/nix.conf
nix profile add nixpkgs#nix-direnv nixpkgs#direnv
mkdir -p ~/.config/direnv
echo 'source $HOME/.nix-profile/share/nix-direnv/direnvrc' >> ~/.config/direnv/direnvrc
```

Depending on your shell, you need to hook direnv differently:
Bash: `echo 'eval "$(direnv hook bash)"' >> ~/.bashrc`
Zsh: `echo 'eval "$(direnv hook zsh)"' >> ~/.zshrc`
Fish: `echo 'direnv hook fish | source' >> ~/.config/fish/config.fish`

Run `direnv allow` to activate direnv for the directory (with the .envrc file) so you load the packages
specified by the nix flake into your shell.

It should be configured now, but you may need to enter a new terminal for the changes to be loaded.

As of right now, this thing lowkey only has devShells and it needs to be expanded but
there won't likely be any additional installation steps.
