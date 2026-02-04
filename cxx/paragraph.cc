#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>

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

	ifstream f(path);
	if (!f) {
		throw system_error(errno, system_category(), path);
	}

	string line;

	while (getline(f, line)) {
		if (line.size() < 7 || line[5] != ':') {
			continue;
		}
		if (line[6] == ' ') {
			string label = line.substr(0, 5);
			ostringstream text;
			text << line.substr(7);
			while (getline(f, line) && !line.empty()) {
				text << '\n' << line;
			}
			paragraphs[label] = paragraph(label, text.str());
		} else if (line.size() >= 12 && line[11] == ' ') {
			string src = line.substr(0, 5);
			auto it = paragraphs.find(src);
			if (it == paragraphs.end()) {
				throw runtime_error("option before paragraph: " + src);
			}
			string dest = line.substr(6, 5);
			string prompt = line.substr(12);
			it->second.options.push_back(option(prompt, dest));
		}
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
	} catch (exception &e) {
		cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
