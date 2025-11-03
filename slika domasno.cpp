#include <iostream>
#include <bits/stdc++.h>

using namespace std;

const int imgsize=128;

struct Pixel{
  int r,g,b;
};

struct Image{
  Pixel pixels[imgsize][imgsize];
};

int main()
{
  Image img;

  int cx = imgsize / 2;
  int cy = imgsize / 2;

  for(int i=0;i<imgsize;i++){
    for(int j=0;j<imgsize;j++){
        int dx = abs(j - cx);
        int dy = abs(i - cy);
        int dist = dx + dy;

        int maxDist = cx + cy;
        double intensity = 1.0 - (double)dist / maxDist;

        img.pixels[i][j].r = 0;
        img.pixels[i][j].g = (int)(255 * max(0.0, intensity));
        img.pixels[i][j].b = 0;
            }
  }

  for (int i = 0; i < imgsize; i++) {
        for (int j = 0; j < imgsize; j++) {

            Pixel p = img.pixels[i][j];
            cout << "\033[38;2;" <<
             p.r << ";" << p.g << ";" << p.b << "m█"; 

        }
        cout << "\033[0m" << endl; 
    }

return 0;
}