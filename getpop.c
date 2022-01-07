/*
Copyright (C) 2021 20kdc & LibreQuake developers

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
// getpop.c - GPL-compliant pop.lmp creator, now with added paranoia!
//
// Defines you can use to control behaviour:
//
// NO_UNLINK: Disables the unlink calls, in case unlink is not available.
//            Always try without this option first, especially on MinGW (which may have unlink in some cases)
// USE_STDOUT: Writes to standard output. This may not be Windows-friendly, so it was avoided.

#include "common.c"
#include <stdio.h>

// Don't import unistd, it might not exist
int unlink(const char * name);

int main(int argc, char ** argv) {
#ifndef USE_STDOUT
	FILE * extract = fopen("pop.lmp", "wb");
	if (!extract) {
		fprintf(stderr, "pop.lmp could not be created\n");
		return 1;
	}
#else
	FILE * extract = stdout;
#endif
	for (int i = 0; i < 128; i ++) {
		unsigned short v = pop[i];
		int err = 0;
		if (fputc(v >> 8, extract) == EOF)
			err = 1;
		if (fputc(v, extract) == EOF)
			err = 1;
		if (err) {
			fprintf(stderr, "failed to complete pop.lmp\n");
#ifndef USE_STDOUT
			fclose(extract);
			// try to erase corrupted file
#ifndef NO_UNLINK
			unlink("pop.lmp");
#endif
#endif
			return 1;
		}
	}
#ifndef USE_STDOUT
	if (fclose(extract)) {
		fprintf(stderr, "pop.lmp closure failed\n");
		// try to erase corrupted file
#ifndef NO_UNLINK
		unlink("pop.lmp");
#endif
		return 1;
	}
#endif
	fprintf(stderr, "pop.lmp created\n");
	return 0;
}

