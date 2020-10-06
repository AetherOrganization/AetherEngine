#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <stack>
#include <sstream>
#include <iomanip>


namespace aeth::profiler
{
	namespace intern
	{
		std::chrono::steady_clock clck;

		struct SectionTimeNode
		{
		public:
			std::string name;
			SectionTimeNode* parent;
			std::vector<SectionTimeNode*> children;
			std::chrono::steady_clock::duration time;

			~SectionTimeNode()
			{
				for (SectionTimeNode* child : children)
				{
					delete child;
				}
			}
		};

		SectionTimeNode rootNode = {
			"Root",
			nullptr,
			{},
			{}
		};

		std::stack<std::chrono::steady_clock::time_point> timeStack; // stores time points for when a section starts
		SectionTimeNode* currentNode = nullptr; // stores the currently-profiled node
		uint32_t biggestNameSize = 11; // start at 11 for "Unspecified"

		void StartFrame() // special behaviour for the root node
		{
			while (timeStack.size() > 0)
			{
				timeStack.pop();
			}

			timeStack.push(clck.now());
			currentNode = &rootNode;
			for (SectionTimeNode* rootChild : currentNode->children)
			{
				delete rootChild;
			}
		}

		void IterateNode(SectionTimeNode* _node, std::stringstream& _os)
		{
			static uint32_t indent = 0;

			std::string indentTree;
			{
				for (uint32_t i = 0; i < indent; i++)
				{
					indentTree += "|";
				}
				indentTree += "-";
			}

			std::string name;
			{
				std::stringstream nameStream;
				nameStream << std::setw(((uint64_t)biggestNameSize + 40ULL) - indent) << std::left;
				nameStream << _node->name;
				name = nameStream.str();
			}

			std::string time;
			{
				std::stringstream timeStream;
				timeStream << std::setw(10) << std::setprecision(1) << std::fixed;
				timeStream << (float)_node->time.count() / 1000000.f;
				time = timeStream.str() + "ms  ";
			}

			if (_node->parent == nullptr)
			{ // special handling for the root node
				_os << indentTree << name << time << "100.0%  100.0%" << std::endl;
			}
			else
			{
				std::string parentString;
				std::string totalString;
				{
					float parentContribution = ((float)_node->time.count() / (float)_node->parent->time.count()) * 100;
					float totalContribution = ((float)_node->time.count() / (float)rootNode.time.count()) * 100;

					std::stringstream percentStream;
					percentStream << std::setw(5) << std::setprecision(1) << std::fixed;

					percentStream << parentContribution;
					parentString = percentStream.str() + "%  ";

					percentStream.str("");

					percentStream << totalContribution;
					totalString = percentStream.str() + "%";
				}
				_os << indentTree << name << time << parentString << totalString << std::endl;
			}

			indent++;
			auto timeLeft = _node->time;
			for (auto childNode : _node->children)
			{
				IterateNode(childNode, _os);
				timeLeft -= childNode->time;
			}

			if (timeLeft.count() > 0)
			{
				std::string unspecName;
				{
					std::stringstream nameStream;
					nameStream << std::setw(((uint64_t)biggestNameSize + 40ULL) - indent) << std::left;
					nameStream << "Unspecified";
					unspecName = nameStream.str();
				}

				std::string unspecTime;
				{
					std::stringstream timeStream;
					timeStream << std::setw(10) << std::setprecision(1) << std::fixed;
					timeStream << (float)timeLeft.count() / 1000000.f;
					unspecTime = timeStream.str() + "ms  ";
				}

				std::string parentString;
				std::string totalString;
				{
					float parentContribution = ((float)timeLeft.count() / (float)_node->time.count()) * 100;
					float totalContribution = ((float)timeLeft.count() / (float)rootNode.time.count()) * 100;

					std::stringstream percentStream;
					percentStream << std::setw(5) << std::setprecision(1) << std::fixed;

					percentStream << parentContribution;
					parentString = percentStream.str() + "%  ";

					percentStream.str("");

					percentStream << totalContribution;
					totalString = percentStream.str() + "%";
				}

				_os << "|" << indentTree << unspecName << unspecTime << parentString << totalString << std::endl;
			}
			indentTree.resize(indentTree.size() - 1);
			_os << indentTree << std::endl;
			indent--;
		}
	}


	void StartSectionProfile(std::string _name)
	{
		intern::timeStack.push(intern::clck.now());
		intern::SectionTimeNode* newNode = new intern::SectionTimeNode;
		newNode->name = _name;
		newNode->parent = intern::currentNode;
		intern::currentNode->children.push_back(newNode);
		intern::currentNode = newNode;

		if (_name.size() > intern::biggestNameSize)
		{
			intern::biggestNameSize = _name.size();
		}
	}

	void StopSectionProfile()
	{
		std::chrono::steady_clock::time_point now = intern::clck.now();
		intern::currentNode->time = now - intern::timeStack.top();
		intern::timeStack.pop();

		intern::currentNode = intern::currentNode->parent;
	}

	std::string GetProfileDataStr()
	{
		std::stringstream strm;

		intern::IterateNode(&intern::rootNode, strm);

		return strm.str();
	}
}
