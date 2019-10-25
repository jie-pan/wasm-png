/*
 * Copyright 2017 Google LLC. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
/* Test harness for JPG transcode */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <stddef.h>
#include <png.h>
#include <zlib.h>


extern int      png_transcode(void *input, int len, int quality);

static void
usage() {
    fprintf(stderr, "pngtopng: usage: pngtopng input-file output-file\n");
    exit(1);
}

#define IMG         "images/js-wa-900.jpg"

int
main(int argc, char *argv[]) {
    int          q, len;
    void         *src;
    FILE         *f, *out;
    struct stat st;

    if (argc < 3) {
        usage();
    }
    q = 100;
    ///
    if ((f = fopen(argv[1], "rb")) == NULL) {
        puts("Barf");
        exit(2);
    }
    if (stat(IMG, &st) != 0) {
        exit(3);
    }
    src = malloc(st.st_size);
    if (src == NULL) {
        exit(4);
    }
    fread(src, st.st_size, 1, f);

    len = png_transcode(src, st.st_size, q);

    out = fopen(argv[2], "wb");
    if (out) {
        fwrite(src, len, 1, out);
        fclose(out);
    }

    fclose(f);

    return 0;
}
