#include <png.h>
#include <stdlib.h>

#define WIDTH 500
#define HEIGHT 500
#define X_MIN -2.5
#define X_MAX 1.0
#define Y_MIN -1.5
#define Y_MAX 1.5
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
      double x0 = X_MIN + (X_MAX - X_MIN) * x / width;
      double y0 = Y_MIN + (Y_MAX - Y_MIN) * y / height;
      double x1 = 0.0;
      double y1 = 0.0;
      int i = 0;
      while (x1 * x1 + y1 * y1 <= 2 * 2 && i < MAX_ITERATIONS) {
        double x2 = x1 * x1 - y1 * y1 + x0;
        double y2 = 2 * x1 * y1 + y0;
        x1 = x2;
        y1 = y2;
        i++;
      }

      // マンデルブロ集合の計算結果を色に変換する
      int color = i * MAX_COLOR_VALUE / MAX_ITERATIONS;
      px[0] = color;
      px[1] = color;
      px[2] = color;
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
