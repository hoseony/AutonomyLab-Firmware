# Writing Docs

1. Install mdbook.
    - for Macos: `brew install mdbook` 
    - main documentation: https://rust-lang.github.io/mdBook/guide/installation.html \
    <small>Sorry if you are using different OS, you can add some documentation for your OS here…</small>
2. To add a page, create a `.md` file in `docs/src/` and list it in `SUMMARY.md`.
3. Preview from the repo root
    ```
    mdbook serve docs --open
    ```
4. Clean the build output:
    ```
    mdbook clean docs
    ```
