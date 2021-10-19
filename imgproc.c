#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Mert Taþ\Downloads\bitmap.h"
/*
MERT TAS

*/
//We're defining the variables that we will use.
BITMAPFILEHEADER bf;
BITMAPINFOHEADER bi;
FILE *f;
FILE *downsized;
FILE *smoothed;
int pix1,pix2,pix3,pix4,avg;
int pix5,pix6,pix7,pix8,pix9,newVal;
float new_value=0;

int main()
{

    downsize();//Assignment 2: Downsizing the image
    smooth();// Assignment 3: Bluring the image

    return 0;
}

void downsize(){

  unsigned char *pixels, *newpix;

  f=fopen("itu.bmp","rb");// Opening the itu.bmp file in read binary mode.
  fread(&bf,sizeof(bf),1,f);// Reads the file header in itu.bmp file
  fread(&bi,sizeof(bi),1,f);// Reads the info header in itu.bmp file

  //bf.bfSize-54 -> The Size of the image only
  pixels=malloc(bf.bfSize-54);//Allocates memory dynamically for pixels array
  //(bf.bfSize-54)/4 -> The Size of the downsized image only
  newpix=malloc((bf.bfSize-54)/4);//Allocates memory dynamically for newpix array.
  fread(pixels,bf.bfSize-54,1,f);//Read the pixel data from itu.bmp file

  //It reads the pixels and accesses the RGB value respectively and takes the avarage of the first 4 ones.
  //Then the avarage is assigned to newpix array.
  int s=0;
  for(int r=1;r<=bi.biHeight;r+=2){

    for(int c=0;c<=bi.biWidth*3-1;c+=6){

     for(int color=0;color<3;color++){
       pix1 = pixels[c+(r-1)*bi.biWidth*3+color];
       pix2 = pixels[c+(r-1)*bi.biWidth*3+3+color];
       pix3 = pixels[c+(r)*bi.biWidth*3+color];
       pix4 = pixels[c+(r)*bi.biWidth*3+3+color];
       avg=(pix1+pix2+pix3+pix4)/4;
       newpix[s]=avg;
       s++;
     }
    }
  }

  downsized=fopen("itu-downscaled.bmp","wb");//Opens the downscaled file.
  bf.bfSize=((bf.bfSize-54)/4)+54;// Defines new bfSize (downscaled image size+headers)
  bi.biHeight=bi.biHeight/2;//new height of the picture
  bi.biWidth=bi.biWidth/2;//new width of the picture

  //Writes the headers and downscaled pix to itu-downsized.bmp file
  fwrite(&bf,sizeof(bf),1,downsized);
  fwrite(&bi,sizeof(bi),1,downsized);
  fwrite(newpix,bf.bfSize-54,1,downsized);
  printf("Downsizing Completed!\n");
  //Free the memory that we've allocated.
  free(pixels);
  free(newpix);
  //Close the opened files.
  fclose(f);
  fclose(downsized);

}

void smooth(){

  unsigned char *pixels, *newpix, *newpix_p;

  f=fopen("itu.bmp","rb");//Open itu.bmp
  fread(&bf,sizeof(bf),1,f);//Reads the file header
  fread(&bi,sizeof(bi),1,f);//Reads the info header

  pixels=malloc(bf.bfSize-54);//Allocates memory dynamically for pixels array
  newpix=malloc((bi.biHeight+2)*(bi.biWidth+2)*3);//Allocates memory dynamically for new pixels array with padding
  newpix_p=malloc((bi.biHeight+2)*(bi.biWidth+2)*3);//Allocates memory dynamically for new pixels array

  fread(pixels,bf.bfSize-54,1,f);//Reads the pixel data

  //For padding it creates a frame with (Height+2)x(Width+2) full of zero
  for(int i=0;i<(bi.biHeight+2)*(bi.biWidth+2)*3;i++){

      newpix[i]=0;
  }
  //Then it adds the pixel data starting from the (1,1) coordinates to make padding completed
  int i=0;
  for(int r=1;r<bi.biHeight;r++){
    for(int c=1;c<=(bi.biWidth+2)*3;c++){

        newpix[(c-1)+(r-1)*(bi.biWidth+2)*3]=pixels[i];
        i++;
    }

  }
  //It reads the 9 pixel's(3x3) RGB values respectively from the padded picture and makes dot product with the kernel for bluring.
  //Then the value is assigned to the newpix_p array.
  for(int r=1;r<=bi.biHeight+2;r++){
    for(int c=0;c<(bi.biWidth+2)*3;c++){

        pix1=newpix[c+(r-1)*bi.biWidth*3];
        pix2=newpix[c+(r-1)*bi.biWidth*3+3];
        pix3=newpix[c+(r-1)*bi.biWidth*3+6];
        pix4=newpix[c+(r)*bi.biWidth*3];
        pix5=newpix[c+(r)*bi.biWidth*3+3];
        pix6=newpix[c+(r)*bi.biWidth*3+6];
        pix7=newpix[c+(r+1)*bi.biWidth*3];
        pix8=newpix[c+(r+1)*bi.biWidth*3+3];
        pix9=newpix[c+(r+1)*bi.biWidth*3+6];
        new_value=(pix1*0.0625+pix2*0.125+pix3*0.0625+pix4*0.125+pix5*0.25+pix6*0.125+pix7*0.0625+pix8*0.125+pix9*0.0625);
        newVal=(int)new_value;
        newpix_p[c+(r-1)*bi.biWidth*3]=newVal;

    }
}

  smoothed=fopen("itu-smoothed.bmp","wb");// Open the file for smoothed pix.
  //Writes the headers and blurred picture
  fwrite(&bf,sizeof(bf),1,smoothed);
  fwrite(&bi,sizeof(bi),1,smoothed);
  fwrite(newpix_p,bf.bfSize-54,1,smoothed);
  printf("Smoothing Completed!\n");
  //Free the memory that we've allocated.
  free(pixels);
  free(newpix);
  free(newpix_p);
  //Close the files.
  fclose(f);
  fclose(smoothed);

}
