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
    /*
     for (int l=0;l<256;l++){
     	cout << (int)histogram[l] ;
     }
     */
    
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

    int total_no_of_pixels = Size1*Size2;
    int no_elements = total_no_of_pixels/256;
    //int bucket[256];

    int counter = histogram[0]; //To Keep the count of number of pixels with a particular intensity value
    unsigned char index = 0; //Variable for storing no of intensisty values i.e 255 
    int bucket = 0; //Intensity values for the new histogram 
    cout << histogram[0]<< endl;
    unsigned char FinalImage[400][400][1];
    for(int i=0;i<400;i++){
        for(int j=0;j<400;j++){
            FinalImage[i][j][0] = 0;
        }
    }
    //Bucket Equalization Logic
    for(int index =0; index < 256; index++) {
        for(int k = 0;k < 400; k++) {
            for(int m=0;m<400;m++){
                if (Imagedata[k][m][0] == index) {

                    if(counter < 625){
                        counter = counter + 1;
                        FinalImage[k][m][0] = bucket;
                    }
                    else {
                        bucket = bucket + 1;
                        counter = 0;
                    }
                    }
                }
            }
        
        //index = index + 1;
    }
    
    double histogram1[256] ;
     for (int i=0;i < 256; i++) {
         histogram1[i] = 0;
       }
     
     for (int k=0;k<400;k++) {
         for (int m=0;m<400;m++) {
        histogram1[FinalImage[k][m][0]]++;
            }
      }     

      ofstream Histogram1;
      Histogram1.open("Histogram_rose_mix_after_Cumulative_equalization.csv");
      for(int i = 0; i < 256; i++){
        Histogram1 << histogram1[i] << endl;
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

