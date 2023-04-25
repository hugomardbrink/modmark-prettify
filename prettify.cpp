#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace nlohmann::literals;

std::vector<std::string> htmlColors{
    "#FF0000",
    "#FC4444",
    "#FC6404",
    "#FCD444",
    "#8CC43C",
    "#029658",
    "#1ABC9C",
    "#5BC0DE",
    "#6454AC",
    "#FC8C84"};

std::vector<std::string> latexColors{
    "red",
    "orange",
    "yellow",
    "green",
    "blue",
    "purple"};

void manifest()
{
  json output = R"(
    {
      "name": "prettify",
      "version": "1.0",
      "description": "This package makes text pretty!",
      "transforms": [
        {
          "from": "prettify",
          "to": ["html", "latex"],
          "arguments": []
        }
      ]
    }
  )"_json;

  std::cout << output.dump() << std::endl;
}

void transformPrettify(std::string to)
{

  std::string input = "", buffer;
  while (std::getline(std::cin, buffer))
    input.append(buffer);

  json inputJson = json::parse(input);
  std::string text = inputJson["data"];
  json output = json::array();

  if (!to.compare("latex"))
  {
    int counter{0};
    std::ostringstream oss;

    oss << R"([{"name": "set-add", "arguments": {"name": "imports"}, "data": "\\usepackage{xcolor}" }, )";
    oss << R"({"name": "raw", "data": ")";

    for (char c : text)
    {
      if (isspace(c))
      {
        oss << " ";
        continue;
      }

      oss << R"(\\textcolor{)" << latexColors.at(counter) << R"(})"
          << R"({)" << c << R"(})";
      counter++;
      counter %= (latexColors.size() - 1);
    }

    oss << R"( "}] )";

    json output = json::parse(oss.str());

    std::cout << output.dump() << std::endl;
  }
  else if (!to.compare("html"))
  {
    int counter{0};
    int index{0};
    for (char c : text)
    {
      if (isspace(c))
      {
        index++;
        output.push_back(json::parse(R"( {"name": "raw", "data": "<span>&nbsp;</span>"} )"));
        continue;
      }

      std::ostringstream oss;

      output.push_back(json::parse(R"( {"name": "raw", "data": ""} )"));
      oss << R"(<span style="color:)" << htmlColors.at(counter) << R"(">)" << c << R"(</span>)";
      output[index]["data"] = oss.str();

      index++;
      counter++;
      counter %= (htmlColors.size() - 1);
    }

    std::cout << output.dump() << std::endl;
  }
  else
    std::cerr << "Cannot convert prettify to: " << to << std::endl;
}

void transform(std::string from, std::string to)
{
  if (!from.compare("prettify"))
    transformPrettify(to);
  else
    std::cerr << "Package does not support: " << from << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    std::cerr << "No arguments found" << std::endl;
    return 1;
  }

  std::string action = argv[1];

  if (!action.compare("manifest"))
    manifest();
  else if (!action.compare("transform"))
    transform(argv[2], argv[3]);
  else
    std::cerr << "Invalid action: " << action << std::endl;

  return 0;
}
