#include "SDL_Wrapper.h"
#include <stdio.h>
#include <string.h>

enum WRAP_SDL{
	WRAP_SDL_INITIALIZED,
	WRAP_SDL_NOT_INITIALIZED
};

SDL_Window * Window = NULL;
SDL_Renderer * Renderer = NULL;
Uint32 RendererFlags = 0;

Uint8 SDLInitialized = WRAP_SDL_NOT_INITIALIZED;

// Load Image
SDL_Surface* LoadImage(const char* ImageLocation)
{
	// Initialize Variables
	SDL_Surface *SDL_S_Return = NULL;

	// Load Image
	SDL_S_Return = IMG_Load(ImageLocation);

	// If it hasn't been loaded...
	if(SDL_S_Return == NULL)
	{
		// Send out an Error Message
		fprintf(stderr, "Error: %s\n", IMG_GetError());
	}

	// Return the Surface (Will be NULL if file isn't loaded)
	return SDL_S_Return;
}

// Create New Image
SDL_Surface* CreateNewImage(int Width, int Height)
{
	return SDL_CreateRGBSurface(0, Width, Height, 24, 0, 0, 0, 0);
}

// Convert To Array
Uint8* GetPixelArray(SDL_Surface* LoadedImage, int *Width, int *Height)
{
	// Initialize Variables
	Uint8* Pixl_Return = NULL;

	// Check if Image is actually loaded
	if(LoadedImage == NULL)
		return NULL;

	// Get Pixels
	Pixl_Return = (Uint8*) LoadedImage->pixels;

	// Set Width and Height
	*Width = LoadedImage->w;
	*Height = LoadedImage->h;

	// Return Array
	return Pixl_Return;
}

// Get Pixel 
Uint8* GetPixelByteAtPosition(SDL_Surface* LoadedImage, int X, int Y)
{
	int bpp = LoadedImage->format->BytesPerPixel;

    return (Uint8*) LoadedImage->pixels + Y * LoadedImage->pitch + X * bpp;
}

// Get RGB Values of a Pixel
void GetRGBValues(SDL_Surface* LoadedImage, int X, int Y, Uint8 *R, Uint8 *G, Uint8 *B)
{
	// Initialize Variables
	Uint32 Pxl_Colour;
	Uint8* Pxl = GetPixelByteAtPosition(LoadedImage, X, Y);

	// Convert the Pixel into Correct Format
	switch(LoadedImage->format->BytesPerPixel) 
	{
    case 1:
        Pxl_Colour = *Pxl;
        break;

    case 2:
        Pxl_Colour = *(Uint16 *) Pxl;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            Pxl_Colour = Pxl[0] << 16 | Pxl[1] << 8 | Pxl[2];
        else
            Pxl_Colour = Pxl[0] | Pxl[1] << 8 | Pxl[2] << 16;
        break;

    case 4:
        Pxl_Colour = *(Uint32 *) Pxl;
        break;

    default:
		Pxl_Colour = 0;
    }

	// Convert into RGB
	SDL_GetRGB(Pxl_Colour, LoadedImage->format, R, G, B);

}

// Get RGB Values of a Pixel
void GetRGBAValues(SDL_Surface* LoadedImage, int X, int Y, Uint8 *R, Uint8 *G, Uint8 *B, Uint8 *A)
{
	// Initialize Variables
	Uint32 Pxl_Colour;
	Uint8* Pxl = GetPixelByteAtPosition(LoadedImage, X, Y);

	// Convert the Pixel into Correct Format
	switch(LoadedImage->format->BytesPerPixel) 
	{
    case 1:
        Pxl_Colour = *Pxl;
        break;

    case 2:
        Pxl_Colour = *(Uint16 *) Pxl;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            Pxl_Colour = Pxl[0] << 24 | Pxl[1] << 16 | Pxl[2] << 8 | Pxl[3];
        else
            Pxl_Colour = Pxl[0] | Pxl[1] << 8 | Pxl[2] << 16 | Pxl[3] << 24;
        break;

    case 4:
        Pxl_Colour = *(Uint32 *) Pxl;
        break;

    default:
		Pxl_Colour = 0;
    }

	// Convert into RGBA
	SDL_GetRGBA(Pxl_Colour, LoadedImage->format, R, G, B, A);
}

// Set RGB Values of a Pixel
void SetRGBValues(SDL_Surface* LoadedImage, int X, int Y, Uint8 R, Uint8 G, Uint8 B)
{
	// Initialize Variables
	Uint32 Pxl_Colour;
    Uint8 *Pxl = GetPixelByteAtPosition(LoadedImage, X, Y);

	// Convert RGB into Correct Pixel Format
	Pxl_Colour = SDL_MapRGB(LoadedImage->format, R, G, B);

	// Set Pixel depending on Bits Per Pixel
    switch(LoadedImage->format->BytesPerPixel) {
    case 1:
        *Pxl = Pxl_Colour;
        break;

    case 2:
        *(Uint16*) Pxl = Pxl_Colour;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            Pxl[0] = (Pxl_Colour >> 16) & 0xff;
            Pxl[1] = (Pxl_Colour >> 8) & 0xff;
            Pxl[2] = Pxl_Colour & 0xff;
        } else {
            Pxl[0] = Pxl_Colour & 0xff;
            Pxl[1] = (Pxl_Colour >> 8) & 0xff;
            Pxl[2] = (Pxl_Colour >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*) Pxl = Pxl_Colour;
        break;
    }
}

void SetRGBAValues(SDL_Surface* LoadedImage, int X, int Y, Uint8 R, Uint8 G, Uint8 B, Uint8 A)
{
	// Initialize Variables
	Uint32 Pxl_Colour;
    Uint8 *Pxl = GetPixelByteAtPosition(LoadedImage, X, Y);

	// Convert RGB into Correct Pixel Format
	Pxl_Colour = SDL_MapRGBA(LoadedImage->format, R, G, B, A);

	// Set Pixel depending on Bits Per Pixel
    switch(LoadedImage->format->BytesPerPixel) {
    case 1:
        *Pxl = Pxl_Colour;
        break;

    case 2:
        *(Uint16*) Pxl = Pxl_Colour;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            Pxl[0] = (Pxl_Colour >> 24) & 0xff;
            Pxl[1] = (Pxl_Colour >> 16) & 0xff;
            Pxl[2] = (Pxl_Colour >> 8) & 0xff;
			Pxl[3] = Pxl_Colour & 0xff;
        } else {
            Pxl[0] = Pxl_Colour & 0xff;
            Pxl[1] = (Pxl_Colour >> 8) & 0xff;
            Pxl[2] = (Pxl_Colour >> 16) & 0xff;
			Pxl[3] = (Pxl_Colour >> 24) & 0xff;
        }
        break;

    case 4:
        *(Uint32*) Pxl = Pxl_Colour;
        break;
    }
}

// Save Image as BMP
void SaveImageBMP(SDL_Surface* LoadedImage, const char * ImageLocation)
{
	SDL_SaveBMP(LoadedImage, ImageLocation);
}

// Create Window
void CreateWindowFlags(const char * WindowName, int Width, int Height, Uint32 WinFlags, Uint32 RendFlags)
{
	if (SDLInitialized == WRAP_SDL_NOT_INITIALIZED)
	{
		if (!SDL_WasInit(SDL_INIT_VIDEO))
			SDL_Init(SDL_INIT_VIDEO);

		SDLInitialized = WRAP_SDL_INITIALIZED;
	}

	if (Window != NULL)
	{
		SDL_SetWindowTitle(Window, WindowName);
		ResizeWindow(Width, Height);
	}
	else
	{
		// Create Window
		Window = SDL_CreateWindow(WindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, WinFlags);

		// Create Renderer
		Renderer = SDL_CreateRenderer(Window, -1, RendFlags);

		// Store Ren
		RendererFlags = RendFlags;
	}

}

// Resize Window
void ResizeWindow(int Width, int Height)
{
	if (SDLInitialized == WRAP_SDL_NOT_INITIALIZED)
	{
		printf("!!! Cannot resize a window that does not exist. Call CreateWindow first. !!!\n");
		printf("Press Enter to Close the Program\n");
		getchar();
		exit(1);
	}

	// Destroy Renderer
	if (Renderer != NULL)
	{
		SDL_DestroyRenderer(Renderer);
	}

	// Resize Window
	SDL_SetWindowSize(Window, Width, Height);

	// (Re)create Renderer
	SDL_CreateRenderer(Window, -1, 0);
}

// Check Close Pressed
Uint8 CheckClosePressed(SDL_Event* Event)
{
	if (Event->type == SDL_QUIT)
	{
		return 1;
	}

	return 0;
}

// Close Window
void CloseWindow()
{
	if (SDLInitialized == WRAP_SDL_INITIALIZED)
	{
		SDL_DestroyRenderer(Renderer);
		SDL_DestroyWindow(Window);
		Window = NULL;
		Renderer = NULL;

		SDLInitialized = WRAP_SDL_NOT_INITIALIZED;
	}
}

// Display Surface
void DisplaySurface(SDL_Surface * SurfaceToDisplay)
{ 
	// Check if the window exists
	if (SDLInitialized == WRAP_SDL_NOT_INITIALIZED)
	{
		printf("!!! Error: Cannot display a surface before creating window. Call CreateWindow first. !!!\n");
		printf("Press Enter to Close the Program\n");
		getchar();
		exit(1);
	}

	SDL_Rect winSize;
	winSize.x = 0;
	winSize.y = 0;

	SDL_GetWindowSize(Window, &winSize.w, &winSize.h);

	int surWidth = SurfaceToDisplay->clip_rect.w;
	int surHeight = SurfaceToDisplay->clip_rect.h;

	// Display warning if the width/height of the surface and the window are different
	if ((winSize.w != surWidth) || (winSize.h != surHeight))
	{
		printf("!!! Warning: Surface has a different width/height than window. !!!\n");
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, SurfaceToDisplay);

	winSize.w = surWidth;
	winSize.h = surHeight;

	SDL_RenderCopy(Renderer, texture, NULL, &winSize);
	SDL_RenderPresent(Renderer);

	SDL_DestroyTexture(texture);
}

// Get Window Surface
SDL_Window* GetWindow()
{
	return Window;
}

SDL_Renderer* GetRenderer()
{
	return Renderer;
}