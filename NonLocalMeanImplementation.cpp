#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;
const float pi=22.0/7.0;

double Gaussian_weight(int n1,int n2){
	double a =1;
	double denominator = sqrt(2*pi)*a;
	double numerator = exp(-((n1*n1 + n2*n2)/(2*a*a)));
	double output = numerator/denominator;
	return output;
}

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
	unsigned char I[Size1][Size2][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(I, sizeof(unsigned char), Size1*Size2*BytesPerPixel, file);
	fclose(file);
    
    unsigned char NewImage[Size1][Size2][BytesPerPixel];

	double h = 10000000;
	double sum1 = 0;
	double sum2 = 0;
	double gaussw = 0;
	double weight1[9];
	double weight2 = 0;
	double weight3 = 0;
	double final_weight =0;
	double sum_of_final_weights = 0;
	int index = 0;
	for(int i = 3; i < 254; i++) {
		for(int j = 3; j<254; j++) {
			for (int k = 0; k < 5; k++){
				for (int l = 0; l < 5; l++){
					for (int n1 = 0; n1<3; n1++){
						for(int n2 =0;n2<3;n2++){
							gaussw = Gaussian_weight(n1,n2);
							//cout << gaussw << endl;
							weight1[index] = gaussw*pow((I[i+n1-1][j+n2-1][0] - I[k+n1-1][l+n2-1][0]),2);
							index = index + 1;
						}
					}
					index = 0;
					for (int d = 0; d < 8; d++){
						weight2 = weight2 + weight1[d]*weight1[d];
						//cout << weight1[d] << "\t" ;
					}	 
					//cout << "\n";
					final_weight = exp(-(sqrt(weight2))/(h*h));
					sum2 = sum2 + I[i+k-2][j+l-2][0]*final_weight;
					sum_of_final_weights = sum_of_final_weights + final_weight;
				}
			}
			NewImage[i][j][0] = sum2/sum_of_final_weights;
			sum2 =0;
			sum_of_final_weights = 0;
		}
	}

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(NewImage, sizeof(unsigned char), Size2*Size1*3, file);
	fclose(file);
    
		return 0;
}


