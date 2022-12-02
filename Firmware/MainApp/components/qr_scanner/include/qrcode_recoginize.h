/*
  * ESPRESSIF MIT License
  *
  * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
  *
  * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
  * it is free of charge, to any person obtaining a copy of this software and associated
  * documentation files (the "Software"), to deal in the Software without restriction, including
  * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
  * to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or
  * substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  */
#ifndef _QRCODE_RECOGINIZE_H_
#define _QRCODE_RECOGINIZE_H_

#include "dl_lib_matrix3d.h"

enum{
	RECONGIZE_OK,
	RECONGIZE_FAIL
};

/**
 * Create QR-code recognize task
 */
void app_qr_recognize(void *pdata);
bool jpg2rgb(camera_fb_t *fb, dl_matrix3du_t **image_matrix);
size_t grayScale(dl_matrix3du_t *image_888,dl_matrix3du_t **gray);
void imageCrop(uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch, dl_matrix3du_t *data, dl_matrix3du_t * dest);

#endif /* _QRCODE_RECOGINIZE_H_ */
