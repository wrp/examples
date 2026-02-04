#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>

class option {
public:
	std::string prompt;
	std::string dest_label;

	option(std::string p, std::string dest) : prompt(p), dest_label(dest) {}

	void display() {
		std::cout << prompt << '\n';
	}
};

class paragraph {
public:
	std::string label;
	std::string text;
	std::vector<option> options;

	paragraph() {}
	paragraph(std::string lbl, std::string t) : label(lbl), text(t) {}

	void display() {
		std::cout << text << '\n';
		int n = 1;
		for (const option &o : options) {
			std::cout << "  " << n << ". " << o.prompt << '\n';
			n += 1;
		}
	}
};

std::map<std::string, paragraph>
load_story(const std::string &path)
{
	std::map<std::string, paragraph> paragraphs;
	std::vector<std::tuple<std::string, std::string, std::string>> pending_options;

	std::ifstream f(path);
	if (!f) {
		throw std::system_error(errno, std::system_category(), path);
	}

	std::regex para_re(R"(^([A-Za-z0-9]+):\s+(.*)$)");
	std::regex opt_re(R"(^([A-Za-z0-9]+):([A-Za-z0-9]+)\s+(.*)$)");
	std::smatch match;
	std::string line;

	while (std::getline(f, line)) {
		if (line.empty()) {
			continue;
		}
		if (std::regex_match(line, match, opt_re)) {
			std::string src = match[1];
			std::string dest = match[2];
			std::string prompt = match[3];
			pending_options.push_back({src, dest, prompt});
		} else if (std::regex_match(line, match, para_re)) {
			std::string label = match[1];
			std::ostringstream text;
			text << match[2].str();
			while (std::getline(f, line) && !line.empty()) {
				text << '\n' << line;
			}
			paragraphs[label] = paragraph(label, text.str());
		}
	}

	for (const auto &[src, dest, prompt] : pending_options) {
		auto it = paragraphs.find(src);
		if (it == paragraphs.end()) {
			std::cerr << "warning: option references unknown paragraph: " << src << '\n';
			continue;
		}
		it->second.options.push_back(option(prompt, dest));
	}

	return paragraphs;
}

void
run_story(std::map<std::string, paragraph> &story, const std::string &start)
{
	std::string current = start;

	while (true) {
		auto it = story.find(current);
		if (it == story.end()) {
			std::cerr << "error: unknown paragraph: " << current << '\n';
			return;
		}
		paragraph &p = it->second;
		std::cout << '\n';
		p.display();

		if (p.options.empty()) {
			std::cout << "\n[The End]\n";
			return;
		}

		std::cout << "\nChoice: ";
		int choice;
		if (!(std::cin >> choice) || choice < 1 || choice > (int)p.options.size()) {
			std::cout << "Invalid choice.\n";
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			continue;
		}
		current = p.options[choice - 1].dest_label;
	}
}

int
main(int argc, char **argv)
{
	std::string path = argc > 1 ? argv[1] : "story";
	std::string start = argc > 2 ? argv[2] : "00001";

	try {
		auto story = load_story(path);
		run_story(story, start);
	} catch (std::system_error &e) {
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
