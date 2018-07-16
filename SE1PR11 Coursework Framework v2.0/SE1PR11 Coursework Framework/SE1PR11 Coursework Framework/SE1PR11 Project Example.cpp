#include "SDL_Wrapper.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
string Location;
SDL_Surface* LoadImage();
void DisplaySurfaceUntilClose(SDL_Surface* Surface);
void BlackandWhite(SDL_Surface* ImageSurface);
void Sepia(SDL_Surface* ImageSurface);
void Brightness(SDL_Surface* ImageSurface);
void Contrast(SDL_Surface* ImageSurface);
void Imageinversion(SDL_Surface* ImageSurface);
void Mirror(SDL_Surface* ImageSurface);
void SavingImage(SDL_Surface* ImageSurface);
Uint8 Truncate(double value);

int main(int argc, char * argv[])
{
	char yesorno;
	cout << "|Image Processing program|                                  |By Alastair Miller|" << endl;

	do
	{
	//Creates a image surface
	SDL_Surface* ImageSurface=LoadImage();

	// Get The Array of the Pixels (Access to the Raw Data Pointer)
	// This is a 1D array, however represents the entire image. 
	int width = 0, height = 0;
	Uint8* ImageArray = GetPixelArray(ImageSurface, &width, &height);

	// Creates a window called 'image' at the same width and height as the original image
	CreateWindow("Original Image", width, height);
	cout << "To continue: close the window, or press ESC in window." << endl;
	DisplaySurfaceUntilClose(ImageSurface); // Display the image in the window
	CloseWindow(); // Close the window (you don't have to do this, this is just an example of what it does).
	cout << endl;
	string menu;
	//Initalises the menu, displays the menu options
cout << "Please enter the desired effect (if you want effect 1. enter 1)" << endl << "1. Grayscale filter" << endl << "2. Sepia filter" << endl << "3. Alter the brightness" <<endl << "4. Alter the contrast" <<endl<< "5. Invert the Image"<<endl<<  "6. Mirror the image"<<  endl;
	do
	{
cin >> menu;
if (menu != "1" && menu != "2" && menu != "3" && menu != "4" && menu != "5" && menu != "6" && menu != "7") {
	cout << "Please enter a valid menu option" << endl;//Prompts the user to enter a valid menu 
}
	} while (menu != "1" && menu != "2" && menu != "3"&& menu != "4"&& menu != "5"&& menu != "6"&&menu != "7");//loops until a valid input is entered

	int menuint = atoi(menu.c_str());//converts the string to an integer for the switch

switch (menuint){
case 1:
	//Loads the black and white function
	BlackandWhite(ImageSurface);
break;
case 2:
	//Loads the sepia function to make the image sepia
	Sepia(ImageSurface);
break;
case 3:
	//Loads the Brightness function to change the brightness of the image
	Brightness(ImageSurface);
	break;
case 4:
	//Loads the Contrast function to change the contrast of the inputted image
	Contrast(ImageSurface);
	break;
case 5:
	//Loads the Inversion filter to invert the image
	Imageinversion(ImageSurface);
	break;
case 6:
	//Applys a mirror effect vertically half way down the page
	Mirror(ImageSurface);
	break;
case 7:
	//Exits the program
	return 0;
}
	
	// Displays the new image
	CreateWindow("Final Image", width, height);
	cout << "To continue: close the window, or press ESC in window." << endl;
	DisplaySurfaceUntilClose(ImageSurface);
	CloseWindow(); // Close the window
	cout << endl;
	// Saves the Image
	SavingImage(ImageSurface);
	// Asks the user if they want to edit another image
	cout << "Do you want to load another image? (Y/N)" << endl;
	cin >> yesorno;
	} while (yesorno == 'Y' || yesorno == 'y' );
	return 0;
}

void DisplaySurfaceUntilClose(SDL_Surface* Surface)
{
	int open = 1; // In C++, one would ordinarily use bool for this. In C, something like this has to be used.
	SDL_Event event;

	// Display the image in the window
	DisplaySurface(Surface);

	// While we aren't closing the window...
	while (open == 1)
	{
		if (SDL_PollEvent(&event))
		{
			// Close the window if it is closed
			if (CheckClosePressed(&event))
			{
				open = 0;
				continue;
			}

			/* This switch statement shows an example of how to check keys that the user has pressed. Most keys on a keyboard can be checked using the SDLK_<key> enum values,
			   for instance SDLK_q specifies the 'q' key, or SDLK_RETURN specifies the return/enter key (not the one on the number pad).
			   
			   If you are interested, many other events can be checked; however you will have to research the SDL 1.2 documentation yourself! */
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					open = 0;
					break;
				default:
					break;
				}
				
				break;
			default:
				break;
			}
		}
	}
}

SDL_Surface* LoadImage() {
	SDL_Surface* ImageSurface;
	int i = 0;
	do
	{
	cout << "Please enter the file location of the image that you want to load" << endl;
	cin >> Location;
	// Load in the Image
	cout << "Loading Image : " << Location << endl;
	ImageSurface = LoadImage(Location.c_str());
	// Check if the image has been loaded correctly
	if (ImageSurface == NULL)
	{
		cout << "!!! Unable to Load Image. !!!" << endl;
		cout << "Press Enter a valid file location" << endl;
	}
	else
	{
		i = 1;
	}
	} while (i==0);
	return ImageSurface;
}

void BlackandWhite(SDL_Surface* ImageSurface) {
	Uint8 R = 0, G = 0, B = 0;
	//loops through the array
	for (int y = ImageSurface->h - 1; y >= 0; y--)
	{
		for (int x = 0; x < ImageSurface->w; x++)
		{
			GetRGBValues(ImageSurface, x, y, &R, &G, &B);
			// Sets the image to grayscale
			R = B = G = (0.299*R+0.587*G+0.114*B);
			// Sets the RGB Value at a Pixel Location
			SetRGBValues(ImageSurface, x, y, R, G, B);
		}
	}
}

void Sepia(SDL_Surface* ImageSurface) {
	// Loops through the image array
	for (int y = ImageSurface->h - 1; y >= 0; y--)
	{
		for (int x = 0; x < ImageSurface->w; x++)
		{
			Uint8 R = 0, G = 0, B = 0;
			//Gets the R G B values from the desired pixel
			GetRGBValues(ImageSurface, x, y, &R, &G, &B);
		//Modifies the R G B values by the desired amount to apply a sepia filter
			R = Truncate((R * .393) + (G * .769) + (B * .189));
			G = Truncate((R * .349) + (G * .686) + (B * .168));
			B = Truncate((R * .272) + (G * .534) + (B * .131));
			
			// Sets the RGB Value at a Pixel Location
			SetRGBValues(ImageSurface, x, y, R, G,B);	
		}
	}
}

void Brightness(SDL_Surface* ImageSurface) {
	int Modifier;
	//User enters the desired brightness value
	cout << "Please enter a brightness modifier (A value between -255 and 255)" << endl;
	cin >> Modifier;
	//Loops through the array.
	for (int y = ImageSurface->h - 1; y >= 0; y--)
	{
		for (int x = 0; x < ImageSurface->w; x++)
		{
			Uint8 R = 0, G = 0, B = 0;
			//Retrieves the pixel values from the array
			GetRGBValues(ImageSurface, x, y, &R, &G, &B);
			//Adds the Brightness value to the R G B values but is limited to 255
			R = Truncate(R + Modifier);
			G = Truncate(G + Modifier);
			B = Truncate(B + Modifier);
			// Sets the RGB Value at a Pixel Location
			SetRGBValues(ImageSurface, x, y, R, G, B);
		}
	}
}

void Contrast(SDL_Surface* ImageSurface) {
	float Truecontrastvalue, modifier;
	do {
		//User enters a constrast value
		cout << "Please enter a contrast value (between -255 and 255)" << endl;
		cin >> modifier;
	} while (modifier<-255 || modifier>255);
	//Loops through the array.
	for (int y = ImageSurface->h - 1; y >= 0; y--)
	{
		for (int x = 0; x < ImageSurface->w; x++)
		{
			Uint8 R = 0, G = 0, B = 0;
			//Contrast calculation
			Truecontrastvalue = (259 * (modifier + 255)) / (255 * (259 - modifier));
			//Retrieves the pixel values from the array
			GetRGBValues(ImageSurface, x, y, &R, &G, &B);
			//Takes away 128 from the R,G and B values before multipling by the true contrast value then finally adding the 128 back on.
			R = Truncate(Truecontrastvalue*(R-128)+128);
			G = Truncate(Truecontrastvalue*(G - 128) + 128);
			B = Truncate(Truecontrastvalue*(B - 128) + 128);
			// Sets the RGB Value at a Pixel Location
			SetRGBValues(ImageSurface, x, y, R, G, B);
		}
	}
}

void Imageinversion(SDL_Surface* ImageSurface) {
	Uint8 R = 0, G = 0, B = 0;
	for (int y = 0; y < ImageSurface->h - 1; y++)
	{
		for (int x = 0; x < ImageSurface->w; x++)
		{
			GetRGBValues(ImageSurface, x, y, &R, &G, &B);
			// Calculates the new R G B values by taking them away from 255
			R = 255-R;
			G = 255 - G;
			B = 255 - B;	
			// Sets the RGB Value at a Pixel Location
			SetRGBValues(ImageSurface, x, y, R, G, B);
		}
	}
}

void Mirror(SDL_Surface* ImageSurface){
	int halfpoint = (ImageSurface->w / 2);//Stores the half point of the image
	for (int y = 0; y < ImageSurface->h - 1; y++) {// loops from top of program to bottom
	int leftcounter = halfpoint - 1, rightcounter = halfpoint + 1;//leftcounter and right counter are intilised
		do
		{
			Uint8 Rl = 0, Gl = 0, Bl = 0, RR = 0, GR = 0, BR = 0;// left and right rgb values
			GetRGBValues(ImageSurface, leftcounter, y, &Rl, &Gl, &Bl);//left counter rgb set
			GetRGBValues(ImageSurface, rightcounter, y, &RR, &GR, &BR);//right counter rgb set
			RR = Rl;//right is set equal to left
			GR = Gl;
			BR = Bl;
			SetRGBValues(ImageSurface, rightcounter, y, RR, GR, BR);
			rightcounter++;//The right counter is moved right while the left counter moves left
			leftcounter--;
		} while (rightcounter < ImageSurface->w && leftcounter>0);//While neither pointer is at the edge of the image
	}
}

void SavingImage(SDL_Surface* ImageSurface) {
	char input;
	do {
		cout << "Do you want to save the new Image? (Y/N)" << endl;
		// Asks the user if they want to save the new image
		cin >> input;
		//Clears the rest of the characters from the stream, so only takes first character
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (input == 'Y' || input == 'y') {
			break;
		}
		else if (input == 'N' || input == 'n') {
			break;
		}
	} while (true);
	if (input == 'Y' || input == 'y') {
		do {
			// Asks the user if they want to overwrite the new image
			cout << "Do you want to overwrite the original image? (Y/N)" << endl;
			cin >> input;
			if (input == 'N' || input == 'n') {
				string Locationcha;
				cout << "Please enter the new file destination to save the image(Make sure you enter the whole file name including .bmp after the desired name):" << endl;
				cin >> Locationcha;
				//User enters the image location for the image to be saved
				const char * Locationchar = Locationcha.c_str();
				SaveImageBMP(ImageSurface, Locationchar);
				SDL_FreeSurface(ImageSurface);
				//To check that the image has saved
				SDL_Surface* TempImage = NULL;
				TempImage = LoadImage(Locationchar);
				cout << "Image saved successfully" << endl;
				break;
			}
			else if (input == 'N' || input == 'n') {
				//Saves image to default image location
				const char * Locationchar = Location.c_str();
				SaveImageBMP(ImageSurface, Locationchar);
				cout << "Image overwritten successfully" << endl;
				break;
			}
		} while (true);


	}
}

Uint8 Truncate(double value) {
	//limits value from 0 -> 255
	if (value > 255) {
		value = 255;
	}
	if (value < 0) {
		value = 0;
	}
	return value;
}