/* Use libarchive to print archive files */
#include <ctype.h>
#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>

int
main(int argc, char *argv[])
{
	struct archive *a;
	struct archive_entry *entry;
	int r;
	const char *filename;

	if (argc < 2) {
		printf("Usage: %s <zipfile>\n", argv[0]);
		return 0;
	}
	filename = argv[1];

	// Create a new archive reading object
	a = archive_read_new();

	// Enable auto-detection of all formats (including zip) and filters
	archive_read_support_format_all(a);
	archive_read_support_filter_all(a);

	r = archive_read_open_filename(a, filename, 10240);
	if (r != ARCHIVE_OK) {
		fprintf(stderr,
			"%s: %s\n",
			filename,
			archive_error_string(a)
		);
		return 1;
	}

	// Iterate through each entry in the archive
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
		printf("Processing file: %s\n", archive_entry_pathname(entry));

		char byte;
		ssize_t bytes_read;
		while ((bytes_read = archive_read_data(a, &byte, 1)) > 0) {
			putchar(byte);
		}
		printf("\n");

		if (bytes_read < 0) {
			fprintf(stderr, "Error reading data: %s\n", archive_error_string(a));
			break;
		}

		// Must skip the rest of the data for this entry before moving to the next header
		archive_read_data_skip(a);
	}

	archive_read_close(a);
	archive_read_free(a);

	return 0;
}
