#pragma once

#include <unordered_set>
#include <unordered_map>


namespace aeth::Args
{
	/// <summary>
	/// Adds a possible kwarg option.
	/// </summary>
	/// <param name="keyword">Keyword to use (the first arg in the pair)</param>
	/// <param name="defaultArg">Value to default to (the value of the second arg if this kwarg is never found).</param>
	void AddKwargHint(std::string keyword, std::string defaultArg);

	/// <summary>
	/// Call once all kwarg hints are specified.
	/// </summary>
	/// <param name="argc"></param>
	/// <param name="argv"></param>
	void ProcessArgs(int argc, char* argv[]);

	/// <summary>
	/// Gets whether an arg is specified or not.
	/// </summary>
	/// <param name="arg">Arg to look for.</param>
	/// <returns>True is the arg was specified.</returns>
	bool GetArg(std::string arg);
	
	/// <summary>
	/// Gets the value of a kwarg.
	/// </summary>
	/// <param name="arg">keyword to look for.</param>
	/// <returns>The value of the kwarg or the default value otherwise</returns>
	std::string GetKwarg(std::string arg);

	/// <summary>
	/// Clean up memory. Should be called at the end of the program.
	/// </summary>
	void Cleanup();
}