# Releasing Steps
1) Compile the solution in Release configuration
2) Specify the current version according to semantic versioning: https://semver.org/
3) Create a release-brach: feature/my-version-number
4) Tag the release-branch
5) Adjust the release number in $pack\CreateRelease.bat to match the above version number
6) Create package with $pack\CreateRelease.bat, but do not commit
7) Grab the package under $pack\my-version-number and release it on github
