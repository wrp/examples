#include <expected>
#include <system_error>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int
readFile(ifstream& file)
{
	string word;
	int c = 0;
	while (file >> word) {
		std::println("{}",  word);
		c += 1;
	}
	return c;
}


struct FileError {
	std::error_code ec;
	std::string path;

	std::string message() const {
		return path + ": " + ec.message();
	}
};


std::expected<int, FileError>
main2(int argc, char **argv)
{
	string path = argc > 1 ? argv[1] : "input";
	std::ifstream f(path);

	if (!f.is_open()) {
		auto ec = std::make_error_code(static_cast<std::errc>(errno));
		return std::unexpected(FileError{ec, path});
	}

	return readFile(f);
}


int
main(int argc, char **argv)
{
	int rc{0};
	auto result = main2(argc, argv);
	if(result) {
		std::println("Read {} words", *result);
	} else {
		FileError fe = result.error();
		std::error_code ec = fe.ec;
		std::println("Error category: {}, Code: {}",
			ec.category().name(), ec.value());

		if (ec == std::errc::no_such_file_or_directory) {
			;
		} else if (ec == std::errc::permission_denied) {
			;
		}
		std::println("{}", fe.message());
		rc = 1;
	}
	return rc;
}
