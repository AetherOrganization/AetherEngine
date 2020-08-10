#include "Args.h"


namespace Args
{
	std::unordered_set<std::string> args;
	std::unordered_map<std::string, std::string> kwargs;

	void AddKwargHint(std::string keyword, std::string defaultArg)
	{
		kwargs[keyword] = defaultArg;
	}

	void ProcessArgs(int argc, char* argv[])
	{
		bool kwarg = false;
		for (int i = 1; i < argc; i++)
		{
			if (kwargs.find(argv[i]) != kwargs.end())
				kwarg = true;
			else if (kwarg)
				kwargs[argv[i - 1]] = argv[i];
			else
				args.insert(argv[i]);
		}
	}

	bool GetArg(std::string arg)
	{
		return args.find(arg) != args.end();
	}

	std::string GetKwarg(std::string arg)
	{
		return kwargs[arg];
	}
}
