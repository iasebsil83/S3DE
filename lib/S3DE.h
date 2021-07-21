#ifndef S3DE_H
#define S3DE_H








// ---------------- DECLARATIONS ----------------

//keyboard constants
enum S3DE_KEYBOARD{
	S3DE_KEY_PRESSED,
	S3DE_KEY_RELEASED
};

enum S3DE_KEYS{
	//function keys
	S3DE_KEY_F1          = 0x101,
	S3DE_KEY_F2          = 0x102,
	S3DE_KEY_F3          = 0x103,
	S3DE_KEY_F4          = 0x104,
	S3DE_KEY_F5          = 0x105,
	S3DE_KEY_F6          = 0x106,
	S3DE_KEY_F7          = 0x107,
	S3DE_KEY_F8          = 0x108,
	S3DE_KEY_F9          = 0x109,
	S3DE_KEY_F10         = 0x10a,
	S3DE_KEY_F11         = 0x10b,
	S3DE_KEY_F12         = 0x10c,

	//command keys
	S3DE_KEY_ESCAPE      = 0x01b,
	S3DE_KEY_END         = 0x16b,
	S3DE_KEY_HOME        = 0x16a,
	S3DE_KEY_PAGE_UP     = 0x168,
	S3DE_KEY_PAGE_DOWN   = 0x169,
	S3DE_KEY_DELETE      = 0x07f,
	S3DE_KEY_BACKSPACE   = 0x008,
	S3DE_KEY_INSERT      = 0x16c,

	//combination keys
	S3DE_KEY_LEFT_SHIFT  = 0x170,
	S3DE_KEY_RIGHT_SHIFT = 0x171,
	S3DE_KEY_LEFT_CTRL   = 0x172,
	S3DE_KEY_RIGHT_CTRL  = 0x173,
	S3DE_KEY_LEFT_ALT    = 0x174,

	//special characters
	S3DE_KEY_BACK_QUOTE  = 0x0b2,
	S3DE_KEY_AMPERSAND   = 0x026,
	S3DE_KEY_DBL_QUOTE   = 0x022, //double quote
	S3DE_KEY_QUOTE       = 0x027,
	S3DE_KEY_UNDERSCORE  = 0x05f,
	S3DE_KEY_TAB         = 0x009,
	S3DE_KEY_CIRCUMFLEX  = 0x05e,
	S3DE_KEY_DOLLAR      = 0x024,
	S3DE_KEY_LESS_THAN   = 0x03c, //<
	S3DE_KEY_GREA_THAN   = 0x03e, //>
	S3DE_KEY_COMA        = 0x02c,
	S3DE_KEY_SEMI_COLON  = 0x03b,
	S3DE_KEY_COLON       = 0x03a,
	S3DE_KEY_EXCLAMATION = 0x021,
	S3DE_KEY_SPACE       = 0x020,
	S3DE_KEY_RETURN      = 0x00d,
	S3DE_KEY_TILDE       = 0x07e,
	S3DE_KEY_HASHTAG     = 0x023,
	S3DE_KEY_PARENTH_B   = 0x028, //parentheses begin
	S3DE_KEY_PARENTH_E   = 0x029, //            end
	S3DE_KEY_BRACKETS_B  = 0x05b, //brackets begin
	S3DE_KEY_BRACKETS_E  = 0x05d, //         end
	S3DE_KEY_BRACES_B    = 0x07b, //braces begin
	S3DE_KEY_BRACES_E    = 0x07d, //       end
	S3DE_KEY_POINT       = 0x02e,
	S3DE_KEY_PARAGRAPH   = 0x0a7,
	S3DE_KEY_DEGREE      = 0x0b0,
	S3DE_KEY_PIPE        = 0x07c,
	S3DE_KEY_BACKSLASH   = 0x05c,
	S3DE_KEY_AT          = 0x040,
	S3DE_KEY_POUND       = 0x0a3,
	S3DE_KEY_DIERESIS    = 0x0a8,
	S3DE_KEY_BLOCK       = 0x0a4,
	S3DE_KEY_ACUTE       = 0x060,
	S3DE_KEY_MU          = 0x0b5,
	S3DE_KEY_QUESTION    = 0x03f,

	//numbers & maths
	S3DE_KEY_0        = 0x030,
	S3DE_KEY_1        = 0x031,
	S3DE_KEY_2        = 0x032,
	S3DE_KEY_3        = 0x033,
	S3DE_KEY_4        = 0x034,
	S3DE_KEY_5        = 0x035,
	S3DE_KEY_6        = 0x036,
	S3DE_KEY_7        = 0x037,
	S3DE_KEY_8        = 0x038,
	S3DE_KEY_9        = 0x039,
	S3DE_KEY_PLUS     = 0x02b,
	S3DE_KEY_MINUS    = 0x02d,
	S3DE_KEY_ASTERISK = 0x02a,
	S3DE_KEY_SLASH    = 0x02f,
	S3DE_KEY_EQUAL    = 0x03d,
	S3DE_KEY_PERCENT  = 0x025,

	//arrows
	S3DE_KEY_LEFT  = 0x164,
	S3DE_KEY_UP    = 0x165,
	S3DE_KEY_RIGHT = 0x166,
	S3DE_KEY_DOWN  = 0x167,

	//capital letters
	S3DE_KEY_A = 0x041,
	S3DE_KEY_B = 0x042,
	S3DE_KEY_C = 0x043,
	S3DE_KEY_D = 0x044,
	S3DE_KEY_E = 0x045,
	S3DE_KEY_F = 0x046,
	S3DE_KEY_G = 0x047,
	S3DE_KEY_H = 0x048,
	S3DE_KEY_I = 0x049,
	S3DE_KEY_J = 0x04a,
	S3DE_KEY_K = 0x04b,
	S3DE_KEY_L = 0x04c,
	S3DE_KEY_M = 0x04d,
	S3DE_KEY_N = 0x04e,
	S3DE_KEY_O = 0x04f,
	S3DE_KEY_P = 0x050,
	S3DE_KEY_Q = 0x051,
	S3DE_KEY_R = 0x052,
	S3DE_KEY_S = 0x053,
	S3DE_KEY_T = 0x054,
	S3DE_KEY_U = 0x055,
	S3DE_KEY_V = 0x056,
	S3DE_KEY_W = 0x057,
	S3DE_KEY_X = 0x058,
	S3DE_KEY_Y = 0x059,
	S3DE_KEY_Z = 0x05a,

	//minimal letters
	S3DE_KEY_a = 0x061,
	S3DE_KEY_b = 0x062,
	S3DE_KEY_c = 0x063,
	S3DE_KEY_d = 0x064,
	S3DE_KEY_e = 0x065,
	S3DE_KEY_f = 0x066,
	S3DE_KEY_g = 0x067,
	S3DE_KEY_h = 0x068,
	S3DE_KEY_i = 0x069,
	S3DE_KEY_j = 0x06a,
	S3DE_KEY_k = 0x06b,
	S3DE_KEY_l = 0x06c,
	S3DE_KEY_m = 0x06d,
	S3DE_KEY_n = 0x06e,
	S3DE_KEY_o = 0x06f,
	S3DE_KEY_p = 0x070,
	S3DE_KEY_q = 0x071,
	S3DE_KEY_r = 0x072,
	S3DE_KEY_s = 0x073,
	S3DE_KEY_t = 0x074,
	S3DE_KEY_u = 0x075,
	S3DE_KEY_v = 0x076,
	S3DE_KEY_w = 0x077,
	S3DE_KEY_x = 0x078,
	S3DE_KEY_y = 0x079,
	S3DE_KEY_z = 0x07a,

	//non-ASCII letters
	S3DE_KEY_A_ACCENT_E = 0x0e9, //acute accentuated E
	S3DE_KEY_G_ACCENT_E = 0x0e8, //grave accentuated E
	S3DE_KEY_G_ACCENT_A = 0x0e0, //grave accentuated A
	S3DE_KEY_G_ACCENT_U = 0x0f9, //grave accentuated U
	S3DE_KEY_CEDILLA_C  = 0x0e7
};

//event constants
enum S3DE_EVENTS{
	S3DE_KEYBOARD,
	S3DE_MOUSE_CLICK,
	S3DE_MOUSE_MOVE,
	S3DE_MOUSE_SCROLL,
	S3DE_DISPLAY,
	S3DE_RESIZE,
	S3DE_TIMER
};

//mouse constants
#define S3DE_MOUSE_PRESSED  0x0000
#define S3DE_MOUSE_RELEASED 0x0001
#define S3DE_LEFT_BUTTON    0x0000
#define S3DE_MIDDLE_BUTTON  0x0001
#define S3DE_RIGHT_BUTTON   0x0002
#define S3DE_SCROLL_UP      0x0003
#define S3DE_SCROLL_DOWN    0x0004

//2D graphics
#define S3DE_EMPTY  0
#define S3DE_FILLED 1

//3D constants
#define S3DE_RENDER_DISTANCE 1000000000 //in pixels
#define S3DE_SKYCOLOR        255        //RGBA color corresponding to (0,  0,  0,255) (perfect black)
#define S3DE_TEXT_FOR_FLOAT   46        //maximum number of characters required to write a float in a .stl file
#define S3DE_BACKSCENE_LIMIT   0        //max z coordinate for the scene behind you
#define S3DE_FRONT 0                    //    to be rendered (in pixels)
#define S3DE_BACK  1
#define S3DE_LEFT  2
#define S3DE_RIGHT 3
#define S3DE_UP    4
#define S3DE_DOWN  5
#define S3DE_STL_BICOLOR_ENABLE         //    to correspond to the scene's scale

//3D point
typedef struct{
	double x,y,z;
} xyz;








// ---------------- 3D ENGINE ----------------

//space rotations & movements
double S3DE_getAngleX();
double S3DE_getAngleY();
void S3DE_addAngleX(double angle);
void S3DE_addAngleY(double angle);
void S3DE_goStraight(double step, char direction);








// ---------------- PLAK MANAGEMENT ----------------

//basics
void S3DE_addPlak(
	double x0,double y0,double z0,
	double x1,double y1,double z1,
	double x2,double y2,double z2,
	int color //in RGBA format (see S3DE_setPixelRGBA())
);
void S3DE_delPlak(int index);








// ---------------- PLAKS / STL FILE CONVERSIONS ----------------
void S3DE_addPlaksFromSTL(char* fileName, int dx,int dy,int dz, int color, double scale);
void S3DE_saveSTLfromPlaks(char* fileName, double scale);








// ---------------- 2D UTILITIES ----------------

//useful
void S3DE_refresh();
void S3DE_fullScreen();
void S3DE_setColor(int r, int v, int b);
void S3DE_setThickness(double thickness);
int S3DE_inScreen(int x,int y);

//graphics
void S3DE_point(double x, double y);
void S3DE_line(double x1,double y1, double x2,double y2);
void S3DE_triangle(double x1,double y1, double x2,double y2, double x3,double y3, int filled);
void S3DE_rectangle(double x1,double y1, double x2,double y2, int filled);
void S3DE_quad(double x1,double y1, double x2,double y2, double x3,double y3, double x4,double y4, int filled);

//text
void S3DE_text(char* text, double size, double x,double y);

//images
void S3DE_imageRGBA(int x,int y, int width,int height, int* data);
int S3DE_setPixelRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

//timed executions
void S3DE_setTimer(int ms);








// ---------------- BASICS -----------------

//init
void S3DE_init(int argc, char** argv, const char* name, unsigned int width,unsigned int height);

//start - stop
void S3DE_start();
void S3DE_stop();








// ---------------- FORMS PATCH ----------------

//plane
#define S3DE_addPlaks_plane( \
	x1, y1, z1,              \
	x2, y2, z2,              \
	x3, y3, z3,              \
	x4, y4, z4,              \
	color                    \
) S3DE_addPlak(x1,y1,z1,x2,y2,z2,x3,y3,z3,color);S3DE_addPlak(x2,y2,z2,x3,y3,z3,x4,y4,z4,~color|255)



//hexaedrons
void S3DE_addPlaks_cuboid(
	int x, int y, int z,
	int width, int height, int depth,
	int color
);

#define S3DE_addPlaks_cube( \
	x,y,z,                  \
	side,                   \
	color                   \
) S3DE_addPlaks_cuboid(x,y,z,side,side,side,color)








#endif
