#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#if 0
#include <uuid/uuid.h>
#endif


void print_user_ID(char* filepath) {
    struct stat sb;
    struct passwd pwent;
    struct passwd *pwentp;
    char buf[_SC_GETPW_R_SIZE_MAX];

    if(stat(filepath, &sb) == -1) {
        perror("stat");
    }

    if (!getpwuid_r(sb.st_uid, &pwent, buf, sizeof(buf), &pwentp))
        printf("%10s\n", pwent.pw_name);
    else
        printf("%7d\n", sb.st_uid);
}


int main(int argc, char **argv) {
	print_user_ID(argc > 1 ? argv[1] : "foo");
}
