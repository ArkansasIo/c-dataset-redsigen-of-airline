# Continuous Integration

The repository now includes `.github/workflows/build.yml`.

## What it does

GitHub Actions runs on pushes and pull requests targeting `master` and:

1. Checks out the repository.
2. Installs Clang.
3. Builds `recordbase` as C++23 with strict warnings.
4. Builds `csv_import` as C17 with warnings enabled.
5. Builds a sanitizer-enabled `recordbase-sanitize` executable using AddressSanitizer and UndefinedBehaviorSanitizer.
6. Runs a basic `recordbase` startup/exit smoke test.
7. Attempts the CSV importer help command without making the build fail if that command is unsupported.

## How to view the results

1. Open the repository on GitHub.
2. Select **Actions**.
3. Select **C++ Build**.
4. Open the latest workflow run.
5. Expand the failed step if a build or test fails; GitHub will show the compiler diagnostics.

## Local equivalent

Linux/macOS/MSYS:

```bash
bash build.sh
```

For the stricter CI-style build, use the commands in `.github/workflows/build.yml`.
