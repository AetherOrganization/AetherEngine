
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

The `master` branch is reserved for finished release versions.</br>
The `pre-build` branch is reserved as a "development" branch, in which finished branches merge into.</br>
For merging into `pre-build` or `master`, you should submit a pull request of your branch.

## Code Conventions
### <ins>Namespaces
All of the code should be within the `Aeth` namespace (short for Aether) however try to be reasonable and create sub-namespaces for different parts of the code (for example the ECS framework code shuld be in `Aeth::Ecs`
