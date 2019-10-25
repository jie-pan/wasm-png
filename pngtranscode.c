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
/*
 * Convert input JPG to output JPG at set quality - brute force, overwrites original.
 */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <png.h>
#include <zlib.h>


/*
 * NB: This overwrites 'buffer'
 */
int
png_transcode(unsigned char *buffer, int len, int quality) {

    png_image image; 
    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_memory(&image, buffer, len) == 0) 
    {
        return -1;
    }

    image.format = PNG_FORMAT_RGBA;

    png_bytep out;
    size_t output_data_length = PNG_IMAGE_SIZE(image);
    out = malloc(output_data_length);
    if (out == NULL)
      return 0;
    memset(out, 0, output_data_length);

    if (png_image_finish_read(&image, NULL /*background*/, out,
                                0 /*row_stride*/, NULL /*colormap*/) == 0) 
    {
        return -1;
    }

    
    png_image wimage;
    memset(&wimage, 0, (sizeof wimage));
    wimage.version = PNG_IMAGE_VERSION;
    wimage.format = PNG_FORMAT_BGRA;
    wimage.height = image.height;
    wimage.width = image.width;

    unsigned long wlength = 0;
    // Get memory size
    bool wresult = png_image_write_to_memory(&wimage, NULL, &wlength, 0, out, 0, NULL);
    if (!wresult)
    {
        fprintf(stderr, "%s\n", image.message);
    }

    // Real write to memory
    unsigned char* wbuffer = (unsigned char*)malloc(wlength);
    if (wbuffer== NULL)
      return 0;

    wresult = png_image_write_to_memory(&wimage, wbuffer, &wlength, 0, out, 0, NULL);

    if(wlength <= len)
    {
        memcpy(buffer, wbuffer, wlength);
    }
    free(out);
    free(wbuffer);

    return wlength;

}

int
png_decode(unsigned char *buffer, int len, int quality) {

    png_image image; 
    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_memory(&image, buffer, len) == 0) 
    {
        return -1;
    }

    image.format = PNG_FORMAT_RGBA;

    png_bytep out;
    size_t output_data_length = PNG_IMAGE_SIZE(image);
    out = malloc(output_data_length);
    if (out == NULL)
      return 0;
    //memset(out, 0, output_data_length);

    if (png_image_finish_read(&image, NULL /*background*/, out,
                                0 /*row_stride*/, NULL /*colormap*/) == 0) 
    {
        return -1;
    }

    return out[0];
}
