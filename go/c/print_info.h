
#ifndef PRINT_INFO_H
#define PRINT_INFO_H
struct Info {
	int n;
	float f;
	const char *s;
};

void print_info(const struct Info *info);

#endif
