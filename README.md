# AetherEngine
General readme for various important things about the project.
Please give this a read over before doing anything with the repo.
## Branch Conventions
When it comes to doing anything, you should open up a branch dedicated to the work you're doing.
The branch name should have one of 3 prefixes:
* `patch-`
	* This should be used when doing a small change or a patch/hot-fix. The name should point to an issue ID like `patch-1`
* `module-`
	* Used for a major module being implemented on the engine. These branches preferably would have branches coming off of them, though not required. The name should be a short description of the module.
* `feature-`
	* Used for smaller features that wouldn't really classify as a whole major module for the engine. Can either be a short description or point to an issueID like patches.

The `master` branch is reserved for finished release versions.
The `pre-build` branch is reserved as a "development" branch, in which finished branches merge into.
Before merging into `pre-build`, you should preferably consult the team.
Before merging into master you should DEFINITELY consult the team.
