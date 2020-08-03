// ---------------- DECLARATIONS ----------------

//special key constants
enum S3DE_KEYBOARD{
	S3DE_KEY_F1        =  -1,
	S3DE_KEY_F2        =  -2,
	S3DE_KEY_F3        =  -3,
	S3DE_KEY_F4        =  -4,
	S3DE_KEY_F5        =  -5,
	S3DE_KEY_F6        =  -6,
	S3DE_KEY_F7        =  -7,
	S3DE_KEY_F8        =  -8,
	S3DE_KEY_F9        =  -9,
	S3DE_KEY_F10       = -10,
	S3DE_KEY_F11       = -11,
	S3DE_KEY_F12       = -12,
	S3DE_KEY_UP        = -13,
	S3DE_KEY_DOWN      = -14,
	S3DE_KEY_LEFT      = -15,
	S3DE_KEY_RIGHT     = -16,
	S3DE_KEY_PAGE_UP   = -17,
	S3DE_KEY_PAGE_DOWN = -18,
	S3DE_KEY_HOME      = -19,
	S3DE_KEY_END       = -20,
	S3DE_KEY_INSERT    = -21,
	S3DE_KEY_BACKSPACE = -22,
	S3DE_KEY_TAB       = -23,
	S3DE_KEY_ENTER     = -24,
	S3DE_KEY_ESCAPE    = -25,
	S3DE_KEY_DELETE    = -26,
	S3DE_KEY_UNDEFINED = -27
};



//mouse constants
enum S3DE_MOUSE{
	S3DE_LEFT_PRESSED    = 0,
	S3DE_LEFT_RELEASED   = 1,
	S3DE_MIDDLE_PRESSED  = 2,
	S3DE_MIDDLE_RELEASED = 3,
	S3DE_RIGHT_PRESSED   = 4,
	S3DE_RIGHT_RELEASED  = 5
};



//3D constants
#define S3DE_RENDER_DISTANCE 1000 //in pixel



//3D point structure
typedef struct{
	int x,y,z;
} xyz;








// ---------------- PLAKS MANAGEMENT ----------------
void S3DE_addPlak(
	int x0,int y0,int z0,
	int x1,int y1,int z1,
	int x2,int y2,int z2,
	int color
);
void S3DE_delPlak(int index);








// ---------------- 2D UTILITIES ----------------

//useful
void S3DE_refresh();
void S3DE_fullScreen();
void S3DE_setColor(int r, int v, int b);
void S3DE_setThickness(float thickness);

//graphics
void S3DE_point(float x, float y);
void S3DE_line(float x1,float y1, float x2,float y2);
void S3DE_triangle(float x1,float y1, float x2,float y2, float x3,float y3, int filled);
void S3DE_rectangle(float x1,float y1, float x2,float y2, int filled);
void S3DE_quad(float x1,float y1, float x2,float y2, float x3,float y3, float x4,float y4, int filled);

//text
void S3DE_text(char* text, float size, float x,float y);

//images
void S3DE_imageRGBA(int x,int y, int width,int height, int* data);
int S3DE_setPixelRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

//timed executions
void S3DE_setTimedExecution(int ms);








// ---------------- BASICS -----------------

//init
void S3DE_init(int argc, char** argv, const char* name, int width,int height);

//start - stop
void S3DE_start();
void S3DE_stop();
