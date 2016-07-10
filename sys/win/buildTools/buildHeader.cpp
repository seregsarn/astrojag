#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <windows.h>
using std::string;

char* include[] = {
	"astrojag_preamble.h",
	"vector.h",
	"topology.h",
	"utility.h",
	"mapTemplate.h",
	"resource.h",
	"logging.h",
	"fov.h",
	"pathfinding.h",
	"rng.h",
	"astrojag_postscript.h",
	0
};

void appendFile(FILE *out, string path) {
	FILE *fp;
	char buf[1024];
	int len;

	//printf("including file %s\n", path.c_str());
	fp = fopen(path.c_str(), "r");
	do {
		len = fread(buf, sizeof(char), 1024, fp);
		//printf("%s", buf);
		fwrite(buf, sizeof(char), len, out);
		//fputs(buf, out);
	} while (!feof(fp));
	fclose(fp);
}

int main(int argc, char **argv) {
	int i;
	string path, searchPath, outPath;
	FILE *header;

	if (argc < 3) {
		perror("You must pass an input path and output path on the command line to use this build tool.\n");
		return 1;
	}
	path = argv[1];
	searchPath = path + "*.h";
	outPath = argv[2];
	header = fopen(outPath.c_str(), "w");
	printf("Creating %s...", outPath.c_str());
	if (!header) {
		fprintf(stderr, "Couldn't create output file %s\n", outPath.c_str());
		return 1;
	}
	for (i = 0; include[i]; i++) {
		appendFile(header, path+include[i]);
	}
	fclose(header);
	//printf("Created %s.\n", outPath.c_str());
	printf(" Done.\n");
	return 0;
}
