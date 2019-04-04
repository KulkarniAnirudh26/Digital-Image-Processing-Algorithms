

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size1 = 390;
	int Size2 = 300;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	}
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size1 = atoi(argv[4]);
			Size2 = atoi(argv[5]);
		}
	}

	// Allocate image data array
	unsigned char Imagedata[Size1][Size2][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size2*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
     //unsigned char Input_Image[Size1][Size2][BytesPerPixel] = Imagedata[Size1][Size2][BytesPerPixel];
     
     //Creating a Histogram 
     double histogram[256] ;
     for (int i=0;i < 256; i++) {
         histogram[i] = 0;
       }
     
     for (int k=0;k<400;k++) {
         for (int m=0;m<400;m++) {
		histogram[Imagedata[k][m][1]]++;
            }
      }  

      //Saving histogram to a .csv 
      /*
      ofstream Histogram;
      Histogram.open("Histogram_rose_dark.csv");
      for (int i=0;i < 256; i++) {
         Histogram << histogram[i] << endl;
       }
      

      ofstream Histogram;
      Histogram.open("Histogram_rose_bright.csv");
      for (int i=0;i < 256; i++) {
         Histogram << histogram[i] << endl;
       }
       */
      ofstream Histogram;
      Histogram.open("Histogram_rose_mix.csv");
      for (int i=0;i < 256; i++) {
         Histogram << histogram[i] << endl;
       }

    
    //Obtaning Probability 
    double prob_histogram[256];
    int total_no_of_pixels = Size1*Size2;
    for (int i = 0; i< 256; i++){
    	prob_histogram[i] = histogram[i]/total_no_of_pixels;
    }
    

    //CDF
    double cdf_histogram[256];
    for (int j = 0; j < 256; j++){
    	if (j == 0) {
    		cdf_histogram[j] = prob_histogram[j];
    	}
    	else {
    		cdf_histogram[j] = prob_histogram[j] + cdf_histogram[j-1];
    	}
    }

    //Converting to 0-255 Range
    unsigned char  transformed_pixel_values[256];
    for (int t =0; t<256;t++) {
    	 transformed_pixel_values[t] = cdf_histogram[t]*255;
    }
    
    
    unsigned char FinalImage[400][400][1];
    for (int k = 0; k <400; k++) {
    	for(int m=0;m<400;m++) {
    		FinalImage[k][m][1] = transformed_pixel_values[Imagedata[k][m][1]];
    	}
    }
   
    //Saving Histogram after Equalization 
    double histogram1[256] ;
     for (int i=0;i < 256; i++) {
         histogram1[i] = 0;
       }
     
     for (int k=0;k<400;k++) {
         for (int m=0;m<400;m++) {
		histogram1[FinalImage[k][m][1]]++;
            }
      }  
    /*
    ofstream Histogram1;
    Histogram1.open("Histogram_rose_dark_after_equalization.csv");
    for (int j =0; j < 256; j++){
    	Histogram1 << histogram1[j] << endl;
    }
    

    ofstream Histogram1;
    Histogram1.open("Histogram_rose_bright_after_equalization.csv");
    for (int j =0; j < 256; j++){
    	Histogram1 << histogram1[j] << endl;
    }
    */

      ofstream Histogram1;
    Histogram1.open("Histogram_rose_mix_after_equalization.csv");
    for (int j =0; j < 256; j++){
      Histogram1 << histogram1[j] << endl;
    }


	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(FinalImage, sizeof(unsigned char), Size1*Size2*BytesPerPixel, file);
	fclose(file);
    
		return 0;
}

