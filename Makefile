CC=emcc
ZLIB_DIR=zlib-1.2.11
PNG_DIR=libpng-1.6.37
CFLAGS=-I$(ZLIB_DIR) -I$(PNG_DIR) -O3 -g 

#all: pngsquash.js 
all: transcode pngsquash.js  pngsquash-simd.js

ZLIB_SRC_FILES := \
	$(ZLIB_DIR)/adler32.c \
	$(ZLIB_DIR)/compress.c \
	$(ZLIB_DIR)/crc32.c \
	$(ZLIB_DIR)/deflate.c \
	$(ZLIB_DIR)/gzclose.c \
	$(ZLIB_DIR)/gzlib.c \
	$(ZLIB_DIR)/gzread.c \
	$(ZLIB_DIR)/gzwrite.c \
	$(ZLIB_DIR)/infback.c \
	$(ZLIB_DIR)/inflate.c \
	$(ZLIB_DIR)/inftrees.c \
	$(ZLIB_DIR)/inffast.c \
	$(ZLIB_DIR)/trees.c \
	$(ZLIB_DIR)/uncompr.c \
	$(ZLIB_DIR)/zutil.c

PNG_SRC_FILES := \
	$(PNG_DIR)/png.c \
	$(PNG_DIR)/pngerror.c \
	$(PNG_DIR)/pngget.c \
	$(PNG_DIR)/pngmem.c \
	$(PNG_DIR)/pngpread.c \
	$(PNG_DIR)/pngread.c \
	$(PNG_DIR)/pngrio.c \
	$(PNG_DIR)/pngrtran.c \
	$(PNG_DIR)/pngrutil.c \
	$(PNG_DIR)/pngset.c \
	$(PNG_DIR)/pngtrans.c \
	$(PNG_DIR)/pngwio.c \
	$(PNG_DIR)/pngwrite.c \
	$(PNG_DIR)/pngwtran.c \
	$(PNG_DIR)/pngwutil.c


SRCS=$(ZLIB_SRC_FILES) $(PNG_SRC_FILES) pngtranscode.c

pngsquash.js: $(SRCS) Makefile
	$(CC) -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 \
		-s EXPORTED_FUNCTIONS="['_png_transcode', '_png_decode']" \
		-Wno-shift-negative-value \
		$(CFLAGS) \
		-lm \
		-o pngsquash.js $(SRCS)

pngsquash-simd.js: $(SRCS) Makefile
	$(CC) -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 \
		-s EXPORTED_FUNCTIONS="['_png_transcode', '_png_decode']" \
		-Wno-shift-negative-value \
	    -msimd128 -s SIMD=1 \
		$(CFLAGS) \
	    -lm \
		-o pngsquash-simd.js $(SRCS)


transcode: $(SRCS) main.c 
	clang -lm -o transcode $(CFLAGS)  $(SRCS) main.c

clean:
	rm transcode
	rm pngsquash*.js
	rm pngsquash*.wasm

