#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>

using namespace std;

class option {
public:
	string prompt;
	string dest_label;

	option(string p, string dest) : prompt(p), dest_label(dest) {}

	void display() {
		cout << prompt << '\n';
	}
};

class paragraph {
public:
	string label;
	string text;
	vector<option> options;

	paragraph() {}
	paragraph(string lbl, string t) : label(lbl), text(t) {}

	void display() {
		cout << text << '\n';
		int n = 1;
		for (const option &o : options) {
			cout << "  " << n << ". " << o.prompt << '\n';
			n += 1;
		}
	}
};

map<string, paragraph>
load_story(const string &path)
{
	map<string, paragraph> paragraphs;
	vector<tuple<string, string, string>> pending_options;

	ifstream f(path);
	if (!f) {
		throw system_error(errno, system_category(), path);
	}

	regex para_re(R"(^([A-Za-z0-9]+):\s+(.*)$)");
	regex opt_re(R"(^([A-Za-z0-9]+):([A-Za-z0-9]+)\s+(.*)$)");
	smatch match;
	string line;

	while (getline(f, line)) {
		if (line.empty()) {
			continue;
		}
		if (regex_match(line, match, opt_re)) {
			string src = match[1];
			string dest = match[2];
			string prompt = match[3];
			pending_options.push_back({src, dest, prompt});
		} else if (regex_match(line, match, para_re)) {
			string label = match[1];
			ostringstream text;
			text << match[2].str();
			while (getline(f, line) && !line.empty()) {
				text << '\n' << line;
			}
			paragraphs[label] = paragraph(label, text.str());
		}
	}

	for (const auto &[src, dest, prompt] : pending_options) {
		auto it = paragraphs.find(src);
		if (it == paragraphs.end()) {
			cerr << "warning: option references unknown paragraph: " << src << '\n';
			continue;
		}
		it->second.options.push_back(option(prompt, dest));
	}

	return paragraphs;
}

void
run_story(map<string, paragraph> &story, const string &start)
{
	string current = start;

	while (true) {
		auto it = story.find(current);
		if (it == story.end()) {
			cerr << "error: unknown paragraph: " << current << '\n';
			return;
		}
		paragraph &p = it->second;
		cout << '\n';
		p.display();

		if (p.options.empty()) {
			cout << "\n[The End]\n";
			return;
		}

		cout << "\nChoice: ";
		int choice;
		if (!(cin >> choice) || choice < 1 || choice > (int)p.options.size()) {
			cout << "Invalid choice.\n";
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}
		current = p.options[choice - 1].dest_label;
	}
}

int
main(int argc, char **argv)
{
	string path = argc > 1 ? argv[1] : "story";
	string start = argc > 2 ? argv[2] : "00001";

	try {
		auto story = load_story(path);
		run_story(story, start);
	} catch (system_error &e) {
		cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
