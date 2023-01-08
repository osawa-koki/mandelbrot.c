#include <png.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITERATIONS 1000
#define MAX_COLOR_VALUE 255

int main() {

  // マンデルブロ集合を描写する
  // 画像の幅と高さを指定する
  int width = WIDTH;
  int height = HEIGHT;

  // 画像のデータを格納する配列を確保する
  png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(sizeof(png_byte) * width * 4);
  }

  // マンデルブロ集合を描写する
  for (int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);

      // マンデルブロ集合の計算を行う
      double a = (double)x / (double)width * 3.5 - 2.5;
      double b = (double)y / (double)height * 2.0 - 1.0;
      double ca = a;
      double cb = b;
      int n = 0;
      while (n < MAX_ITERATIONS) {
        double aa = a * a - b * b;
        double bb = 2 * a * b;
        a = aa + ca;
        b = bb + cb;
        if (a * a + b * b > 4.0) {
          break;
        }
        n++;
      }

      // マンデルブロ集合の計算結果を元に画素の色を決定する
      int brightness = 0;
      if (n == MAX_ITERATIONS) {
        brightness = 0;
      } else {
        brightness = MAX_COLOR_VALUE * n / MAX_ITERATIONS;
      }
      px[0] = brightness;
      px[1] = brightness;
      px[2] = brightness;
      px[3] = MAX_COLOR_VALUE;
    }
  }

  // 画像をファイルに出力する
  FILE *fp = fopen("output.png", "wb");
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);
  png_init_io(png, fp);
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  png_write_info(png, info);
  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  // メモリを解放する
  for (int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  return 0;

}
