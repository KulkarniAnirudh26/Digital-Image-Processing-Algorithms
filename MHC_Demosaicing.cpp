#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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
	unsigned char Imagedata0[Size2][Size1][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata0, sizeof(unsigned char), Size1*Size2*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    //Boundary Condition--Extending booundary of the image 
    //Creating a New Image "Image2" 
    unsigned char Imagedata[Size2+2][Size1+2][BytesPerPixel];
    for(int i = 0; i<Size2; i++){
    	for (int j =0; j<Size1; j++) {
    		Imagedata[i+1][j+1][0] = Imagedata0[i][j][0];
    	}
    }
    for(int k =0; k< Size2+2 ;k++){
    	Imagedata[k][0][0] = Imagedata0[k][0][0];
    	Imagedata[k][Size1+1][0] = Imagedata0[k][Size1][0];
    }

    for(int m =0; m< Size1+2 ;m++){
    	Imagedata[0][m][0] = Imagedata0[0][m][0];
    	Imagedata[Size2+1][m][0] = Imagedata0[Size2][m][0];
    }
    
    //Creating New Images[data arrays] to hold Red,Green and Blue colour values
    unsigned char RedImagedata[Size2][Size1][1];
	unsigned char BlueImagedata[Size2][Size1][1];
	unsigned char GreenImagedata[Size2][Size1][1];

	//Initializing them to Zero
	for(int i = 0; i <300; i++) {
		for(int j= 0; j<390;j++) {
			BlueImagedata[i][j][0] = 0;
			RedImagedata[i][j][0] = 0;
			GreenImagedata[i][j][0] = 0;
		}
	}

	//Applying the Linear filters
    
    for(int i=0;i<300;i++) {
		for(int j=0;j<390;j++) {
			if ((i+j)%2 == 0 ) { //Green Pixels
				GreenImagedata[i][j][0] = Imagedata[i][j][0];
				//cout << (int)GreenImagedata[i][j][0] << endl;
				if ((i%2 == 1) && (j%2 == 1)){
					//cout << "G1" << endl;
					BlueImagedata[i][j][0] = (5.0*Imagedata[i][j][0] + 4.0*Imagedata[i][j-1][0] + 4.0*Imagedata[i][j+1][0] - Imagedata[i-1][j-1][0] - Imagedata[i][j-2][0] - Imagedata[i][j+2][0] - Imagedata[i-1][j+1][0] - Imagedata[i+1][j-1][0] - Imagedata[i+1][j+1][0] + 0.5*Imagedata[i-2][j][0] + 0.5*Imagedata[i+2][j][0])/9.0;
					RedImagedata[i][j][0] = (5.0*Imagedata[i][j][0] + 4.0*Imagedata[i][j-1][0] + 4.0*Imagedata[i][j+1][0] - Imagedata[i-1][j-1][0] - Imagedata[i][j-2][0] - Imagedata[i][j+2][0] - Imagedata[i-1][j+1][0] - Imagedata[i+1][j-1][0] - Imagedata[i+1][j+1][0] + 0.5*Imagedata[i-2][j][0] + 0.5*Imagedata[i+2][j][0])/9.0;
				}
				if ((i%2 == 0) && (j%2 == 0)){
					BlueImagedata[i][j][0] = (5.0*Imagedata[i][j][0] + 4.0*Imagedata[i-1][j][0] + 4.0*Imagedata[i+1][j][0] - Imagedata[i-1][j-1][0] - Imagedata[i-2][j][0] - Imagedata[i-1][j+1][0] - Imagedata[i+1][j+1][0] - Imagedata[i+2][j][0] - Imagedata[i+1][j-1][0] + 0.5*Imagedata[i][j-2][0] + 0.5*Imagedata[i][j+2][0])/9.0;
					RedImagedata[i][j][0] = (5.0*Imagedata[i][j][0] + 4.0*Imagedata[i-1][j][0] + 4.0*Imagedata[i+1][j][0] - Imagedata[i-1][j-1][0] - Imagedata[i-2][j][0] - Imagedata[i-1][j+1][0] - Imagedata[i+1][j+1][0] - Imagedata[i+2][j][0] - Imagedata[i+1][j-1][0] + 0.5*Imagedata[i][j-2][0] + 0.5*Imagedata[i][j+2][0])/9.0;
					//cout << "G2"  << endl;
				}

			}
		    if ((i%2 == 0) && (j%2 == 1)) { //Red Pixels
		    	RedImagedata[i][j][0] = Imagedata[i][j][0];
		    	//cout << (int)RedImagedata[i][j][0] << endl;
		    	GreenImagedata[i][j][0] = (2.0*Imagedata[i][j-1][0] + 2.0*Imagedata[i-1][j][0] + 2.0*Imagedata[i][j+1][0] + 2.0*Imagedata[i+1][j][0]+ 4.0*Imagedata[i][j][0] - Imagedata[i-2][j][0] - Imagedata[i+2][j][0] - Imagedata[i][j-2][0] - Imagedata[i][j+2][0])/8.0;
		    	BlueImagedata[i][j][0] =  (6.0*Imagedata[i][j][0] + 2.0*Imagedata[i-1][j-1][0] + 2.0*Imagedata[i-1][j+1][0] + 2.0*Imagedata[i+1][j-1][0] + 2.0*Imagedata[i+1][j+1][0] - 1.5*Imagedata[i-2][j][0] - 1.5*Imagedata[i][j+2][0] - 1.5*Imagedata[i+2][j][0] - 1.5*Imagedata[i][j-2][0])/4.0;
		    	//cout << "R" << endl;
		    }
			if ( (i%2==1) && (j%2 == 0)){ //Blue Pixels
				BlueImagedata[i][j][0] = Imagedata[i][j][0];
				//cout << (int)BlueImagedata[i][j][0] << endl;
				GreenImagedata[i][j][0] = (2.0*Imagedata[i][j-1][0] + 2.0*Imagedata[i-1][j][0] + 2.0*Imagedata[i][j+1][0] + 2.0*Imagedata[i+1][j][0]+ 4.0*Imagedata[i][j][0] - Imagedata[i-2][j][0] - Imagedata[i+2][j][0] - Imagedata[i][j-2][0] - Imagedata[i][j+2][0])/8.0;
				RedImagedata[i][j][0] = (6.0*Imagedata[i][j][0] + 2.0*Imagedata[i-1][j-1][0] + 2.0*Imagedata[i-1][j+1][0] + 2.0*Imagedata[i+1][j-1][0] + 2.0*Imagedata[i+1][j+1][0] - 1.5*Imagedata[i-2][j][0] - 1.5*Imagedata[i][j+2][0] - 1.5*Imagedata[i+2][j][0] - 1.5*Imagedata[i][j-2][0])/4.0;
				//cout << "B" << endl;
		}
	}
}
	//Stacking the Red,Green,Blue Images over each other to get the Final Image
	unsigned char FinalImage[Size2][Size1][3]; 
    //cout << " \n At the end now" << endl;
	for(int c =0; c < 3; c++) {
		for(int a=0; a < 300; a++ ) {
			for(int b =0; b < 390;b++) {
			if (c == 0) {
				FinalImage[a][b][c] = BlueImagedata[a][b][0];
				
			}
			if (c == 1) {
				FinalImage[a][b][c] = GreenImagedata[a][b][0];
						}
			if (c == 2) {
				FinalImage[a][b][c] = RedImagedata[a][b][0];
						}
		}
	   }
	}

	
	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(FinalImage, sizeof(unsigned char), (Size2+2)*(Size1+2)*3, file);
	fclose(file);
    
		return 0;
}
