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
	auto result = main2(argc, argv)
		.and_then([](int c) -> std::expected<int, FileError> {
		std::println("Read {} words", c);
		return 0;
	});
	if (!result) {
		std::println("{}", result.error().message());
		return 1;
	}
	return result.value_or(1);
}
