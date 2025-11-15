// ==========================================================
// alterIWnet project
// 
// Component: aiw_client
// Sub-component: steam_api
// Purpose: IW5 function definitions
//
// Initial author: NTAuthority
// Started: 2012-01-29
// ==========================================================

#pragma once

typedef enum {
	K_TAB = 9,
	K_ENTER = 13,
	K_ESCAPE = 27,
	K_SPACE = 32,

	K_BACKSPACE = 127,

	K_COMMAND = 128,
	K_CAPSLOCK = 129,
	K_POWER = 152,
	K_PAUSE,

	K_UPARROW,//154
	K_DOWNARROW,
	K_LEFTARROW,
	K_RIGHTARROW,

	K_ALT,//158
	K_CTRL,
	K_SHIFT,
	K_INS,
	K_DEL,
	K_PGDN,
	K_PGUP,
	K_HOME,
	K_END,

	K_F1,//167
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_F13,
	K_F14,
	K_F15,

	K_KP_HOME,//182
	K_KP_UPARROW,
	K_KP_PGUP,
	K_KP_LEFTARROW,
	K_KP_5,
	K_KP_RIGHTARROW,
	K_KP_END,
	K_KP_DOWNARROW,
	K_KP_PGDN,
	K_KP_ENTER,
	K_KP_INS,
	K_KP_DEL,
	K_KP_SLASH,
	K_KP_MINUS,
	K_KP_PLUS,
	K_KP_NUMLOCK,
	K_KP_STAR,
	K_KP_EQUALS,

	K_MOUSE1, //200
	K_MOUSE2,
	K_MOUSE3,
	K_MOUSE4,
	K_MOUSE5,

	K_MWHEELDOWN, //205
	K_MWHEELUP,

	K_JOY1,
	K_JOY2,
	K_JOY3,
	K_JOY4,
	K_JOY5,
	K_JOY6,
	K_JOY7,
	K_JOY8,
	K_JOY9,
	K_JOY10,
	K_JOY11,
	K_JOY12,
	K_JOY13,
	K_JOY14,
	K_JOY15,
	K_JOY16,
	K_JOY17,
	K_JOY18,
	K_JOY19,
	K_JOY20,
	K_JOY21,
	K_JOY22,
	K_JOY23,
	K_JOY24,
	K_JOY25,
	K_JOY26,
	K_JOY27,
	K_JOY28,
	K_JOY29,
	K_JOY30,
	K_JOY31,
	K_JOY32,

	K_AUX1,
	K_AUX2,
	K_AUX3,
	K_AUX4,
	K_AUX5,
	K_AUX6,
	K_AUX7,
	K_AUX8,
	K_AUX9,
	K_AUX10,
	K_AUX11,
	K_AUX12,
	K_AUX13,
	K_AUX14,
	K_AUX15,
	K_AUX16,

	K_LAST_KEY		// this had better be <256!
} keyNum_t;

struct LocalizeEntry
{
	const char* value;
	const char* name;
};

typedef enum
{
	DVAR_TYPE_BOOL		= 0,
	DVAR_TYPE_FLOAT		= 1,
	DVAR_TYPE_FLOAT_2	= 2,
	DVAR_TYPE_FLOAT_3	= 3,
	DVAR_TYPE_FLOAT_4	= 4,
	DVAR_TYPE_INT		= 5,
	DVAR_TYPE_ENUM		= 6,
	DVAR_TYPE_STRING	= 7,
	DVAR_TYPE_COLOR		= 8,
	TYPE_VEC3_2 = 9
} dvar_type;

typedef enum
{
	DVAR_FLAG_NONE = 0x0,			//no flags
	DVAR_FLAG_SAVED = 0x1,			//saves in config_mp.cfg for clients
	DVAR_FLAG_LATCHED = 0x2,			//no changing apart from initial value (although it might apply on a map reload, I think)
	DVAR_FLAG_CHEAT = 0x4,			//cheat
	DVAR_FLAG_REPLICATED = 0x8,			//on change, this is sent to all clients (if you are host)
	DVAR_FLAG_UNKNOWN10 = 0x10,			//unknown
	DVAR_FLAG_UNKNOWN20 = 0x20,			//unknown
	DVAR_FLAG_UNKNOWN40 = 0x40,			//unknown
	DVAR_FLAG_SERVER = 0x80,			//server dvar
	DVAR_FLAG_USERCREATED = 0x100,		//a 'set' type command created it
	DVAR_FLAG_USERINFO = 0x200,		//userinfo?
	DVAR_FLAG_SERVERINFO = 0x400,		//in the getstatus oob
	DVAR_FLAG_WRITEPROTECTED = 0x800,		//write protected
	DVAR_FLAG_UNKNOWN1000 = 0x1000,		//unknown
	DVAR_FLAG_READONLY = 0x2000,		//read only (same as 0x800?)
	DVAR_FLAG_UNKNOWN4000 = 0x4000,		//unknown
	DVAR_FLAG_UNKNOWN8000 = 0x8000,		//unknown
	DVAR_FLAG_UNKNOWN10000 = 0x10000,		//unknown
	DVAR_FLAG_DEDISAVED = 0x1000000,		//unknown
	DVAR_FLAG_NONEXISTENT = 0xFFFFFFFF	//no such dvar
} dvar_flag;

union dvar_value_t {
	char*	string;
	int		integer;
	float	value;
	bool	boolean;
	float	vec2[2];
	float	vec3[3];
	float	vec4[4];
	BYTE	color[4];
};

union dvar_maxmin_t {
	int i;
	float f;
};

typedef struct dvar_t
{
	const char*		name;
	short			flags;
	char			pad1[2];
	char			type;
	char			pad2[3];
	dvar_value_t	current;
	dvar_value_t	latched;
	dvar_value_t	default;
	dvar_maxmin_t min;
	dvar_maxmin_t max;
} dvar_t;

struct cmd_functions_s
{
	cmd_functions_s *next;
	const char *name;
	const char *autoCompleteDir;
	const char *autoCompleteExt;
	void(__cdecl *function)();
};

typedef struct  
{
	int unknown1;
	int unknown2;
	char* data;
	int unknown3;
	int maxsize; // 16
	int cursize;
	int unknown4;
	int readcount; // 28
	char pad[12];
} msg_t;

typedef struct
{
	char pad[12];
} cmd_function_s;

// netadr_t
typedef enum {
	NA_BOT,
	NA_BAD,					// an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	//NA_IP6,
} netadrtype_t;

typedef enum {
	NS_CLIENT,
	NS_SERVER
} netsrc_t;

typedef struct {
	netadrtype_t	type;

	BYTE	ip[4];

	unsigned short	port;

	BYTE	ipx[10];
} netadr_t;

typedef struct gentity_s
{
	char pad[628];
} gentity_t;

extern gentity_t* g_entities;

enum scriptType_e
{
	SCRIPT_NONE = 0,
	SCRIPT_OBJECT = 1,
	SCRIPT_STRING = 2,
	SCRIPT_VECTOR = 4,
	SCRIPT_FLOAT = 5,
	SCRIPT_INTEGER = 6
};

typedef struct  
{
	union
	{
		void* entity;
		float number;
		unsigned short string;
		float* vector;
		int integer;
	};
	scriptType_e type;
} VariableValue;




struct GfxImage
{
	char* texture;
	char unknown2;
	char a3;
	char a2;
	char unknown3;
	char unknown4;
	char unknown5;
	char unknown6;
	char a4;
	int dataLength1;
	int dataLength2;
	short height;
	short width;
	short depth;
	short unknown8;
	char* name;
};

struct MaterialTextureDef
{
	unsigned int typeHash; // asset hash of type
	char firstCharacter; // first character of image name
	char secondLastCharacter; // second-last character of image name (maybe only in CoD4?!)
	unsigned char unknown; // maybe 0xE2
	char unknown2; // likely 0x00
	GfxImage* image; // GfxImage* actually
};

struct IDirect3DVertexShader9;
struct IDirect3DPixelShader9;
struct IDirect3DTexture9;
struct IDirect3DVertexDeclaration9;

struct VertexDecl
{
	const char* name;
	int unknown;
	char pad[28];
	IDirect3DVertexDeclaration9* declarations[16];
};

struct PixelShader
{
	const char* name;
	IDirect3DPixelShader9* shader;
	DWORD* bytecode;
	int flags;
};

struct VertexShader
{
	const char* name;
	IDirect3DVertexShader9* shader;
	DWORD* bytecode;
	int flags;
};

struct MaterialPass
{
	VertexDecl* vertexDecl;
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	char pad[8];
};

struct MaterialTechnique
{
	int pad;
	short pad2;
	short numPasses;
	MaterialPass passes[1];
};

struct MaterialTechniqueSet
{
	const char* name;
	char pad[4];
	MaterialTechniqueSet* remappedTechniqueSet;
	MaterialTechnique* techniques[48];
};

struct Material
{
	const char* name;
	unsigned short flags; // 0x2F00 for instance
	unsigned char animationX; // amount of animation frames in X
	unsigned char animationY; // amount of animation frames in Y
	char unknown1[4]; // 0x00
	unsigned int rendererIndex; // only for 3D models
	char unknown9[4];
	unsigned int surfaceTypeBits;
	unsigned int unknown2; // 0xFFFFFFFF
	unsigned int unknown3; // 0xFFFFFF00
	char unknown4[40]; // 0xFF
	char numMaps; // 0x01, possibly 'map count' (zone code confirms)
	char unknown5; // 0x00
	char unknownCount2; // 0x01, maybe map count actually
	char unknown6; // 0x03
	unsigned int unknown7; // 0x04
	MaterialTechniqueSet* techniqueSet; // '2d' techset; +80
	MaterialTextureDef* maps; // map references
	unsigned int unknown8;
	void* stateMap; // might be NULL, need to test
};

typedef struct fontEntry_s
{
	unsigned short character;
	unsigned char padLeft;
	unsigned char padTop;
	unsigned char padRight;
	unsigned char width;
	unsigned char height;
	unsigned char const0;
	float uvLeft;
	float uvTop;
	float uvRight;
	float uvBottom;
} fontEntry_t;

typedef struct Font_s
{
	char* name;
	int size;
	int entries;
	Material* image;
	Material* glowImage;
	fontEntry_t* characters;
} Font;


typedef float vec4_t[4];

/*typedef void (__cdecl * MSG_Init_t)(msg_t* msg, char* data, int maxsize);
extern MSG_Init_t MSG_Init;*/

typedef void* (__cdecl * DB_FindXAssetHeader_t)(int type, const char* filename, bool);
extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

typedef void (__cdecl * Dvar_SetCommand_t)(const char* var, const char* value);
extern Dvar_SetCommand_t Dvar_SetCommand;

typedef void (__cdecl * Cbuf_AddText_t)(int controller, const char* command);
extern Cbuf_AddText_t Cbuf_AddText;

typedef int (__cdecl * FS_ReadFile_t)(const char* filename, void** buffer);
extern FS_ReadFile_t FS_ReadFile;

typedef void (__cdecl * Con_Print_t)(int controller, int channel, const char* string, int a4, int a5, float a6);
extern Con_Print_t Con_Print;

typedef dvar_t* (__cdecl * Dvar_FindVar_t)(const char* name);
extern Dvar_FindVar_t Dvar_FindVar;

typedef void (__cdecl * Com_Error_t)(int type, const char* format, ...);
extern Com_Error_t Com_Error;

typedef void (__cdecl * Scr_AddInt_t)(int integer);
extern Scr_AddInt_t Scr_AddInt;

typedef void (__cdecl * Scr_AddFloat_t)(float value);
extern Scr_AddFloat_t Scr_AddFloat;

typedef void (__cdecl * Scr_AddString_t)(const char* string);
extern Scr_AddString_t Scr_AddString;

typedef void (__cdecl * Scr_AddEntityNum_t)(int entityNum, int type);
extern Scr_AddEntityNum_t Scr_AddEntityNum;

typedef short (__cdecl * SL_GetString_t)(const char* string, int a2);
extern SL_GetString_t SL_GetString;

typedef void (__cdecl * Scr_NotifyNum_t)(int number, int type, short notify, int numArgs);
extern Scr_NotifyNum_t Scr_NotifyNum;

typedef void (__cdecl * Scr_AddVector_t)(float* vector);
extern Scr_AddVector_t Scr_AddVector;

typedef char* (__cdecl * SL_ConvertToString_t)(short);
extern SL_ConvertToString_t SL_ConvertToString;

//typedef int (__cdecl * Scr_GetNumParam_t)();
//extern Scr_GetNumParam_t Scr_GetNumParam;
int Scr_GetNumParam();

typedef const char* (__cdecl * Scr_GetString_t)(int index);
extern Scr_GetString_t Scr_GetString;

typedef void (__cdecl * Cmd_AddCommand_t)(const char*, void (*)(), cmd_function_s*);
extern Cmd_AddCommand_t Cmd_AddCommand;

typedef void (__cdecl * SV_GameSendServerCommand_t)(int targetEntity, int a2, const char* command);
extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

typedef void (__cdecl * Cmd_TokenizeString_t)(char* token);
extern Cmd_TokenizeString_t Cmd_TokenizeString;

typedef void (__cdecl * Cmd_EndTokenizedString_t)();
extern Cmd_EndTokenizedString_t Cmd_EndTokenizedString;

typedef void (__cdecl * Scr_NotifyLevel_t)(short notify, int numArgs);
extern Scr_NotifyLevel_t Scr_NotifyLevel;

typedef void (__cdecl * FS_Printf_t)(int file, char* fmt, ...);
extern FS_Printf_t FS_Printf;

/*typedef char* (__cdecl * Key_KeynumToString_t)(int key, int a2);
extern Key_KeynumToString_t Key_KeynumToString;
*/
typedef void(__cdecl * Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

typedef void ExConsolePrint(const char* msg);
extern ExConsolePrint* exprint;

typedef void*(__cdecl * R_RegisterFont_t)(const char*);
extern R_RegisterFont_t R_RegisterFont;

typedef int(__cdecl * R_TextWidth_t)(const char* text, int maxlength, Font* font);
extern R_TextWidth_t R_TextWidth;

typedef void(__cdecl * R_AddCmdDrawText_t)(const char* text, int, void* font, float screenX, float screenY, float, float, float rotation, float* color, int);
extern R_AddCmdDrawText_t R_AddCmdDrawText;

typedef void*(__cdecl * Material_RegisterHandle_t)(const char*);
extern Material_RegisterHandle_t Material_RegisterHandle;

typedef void(__cdecl *R_AddCmdDrawStretchPicInternal_t)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, const float *color, void* material);
extern R_AddCmdDrawStretchPicInternal_t R_AddCmdDrawStretchPicInternal;

typedef void*(__cdecl * UI_DrawHandlePic_t)(void* placement, float x, float y, float width, float height, int XRect, int YRect, const vec4_t color, void* material);
extern UI_DrawHandlePic_t UI_DrawHandlePic;

typedef void*(__cdecl * ScrPlace_GetActivePlacement_t)();
extern ScrPlace_GetActivePlacement_t ScrPlace_GetActivePlacement;

typedef int(__cdecl * Com_Milliseconds_t)(void);
extern Com_Milliseconds_t Com_Milliseconds;

typedef void(__cdecl *UI_OpenMenu_t)(int ClientNumber, char* MenuName);
extern UI_OpenMenu_t UI_OpenMenu;

typedef void(__cdecl* SetErrorTextMessage_t)(char *message);
extern SetErrorTextMessage_t SetErrorTextMessage;

typedef bool*(__cdecl * Key_IsCatcherActive_t)(std::uintptr_t, int);
extern Key_IsCatcherActive_t Key_IsCatcherActive;

typedef void*(__cdecl * Key_SetCatcher_t)(std::uintptr_t, int);
extern Key_SetCatcher_t Key_SetCatcher;

typedef void*(__cdecl * Key_RemoveCatcher_t)(std::uintptr_t, int);
extern Key_RemoveCatcher_t Key_RemoveCatcher;

typedef void(__cdecl*CL_KeyEvent_t)(int a1, int key, int down, int a4);
extern CL_KeyEvent_t CL_KeyEvent;

typedef bool*(__cdecl * Key_IsDown_t)(int, int);
extern Key_IsDown_t Key_IsDown;

typedef void(__cdecl *Dvar_ForEachName_t)(void(__cdecl *callback)(dvar_t *, void *), void *userData);
extern Dvar_ForEachName_t Dvar_ForEachName;

typedef dvar_t*(__cdecl *Dvar_RegisterBool_t)(char *name, int value, int type);
extern Dvar_RegisterBool_t Dvar_RegisterBool;

typedef dvar_t* (__cdecl * Dvar_RegisterInt_t)(const char* name, int default, int min, int max, int flags, const char* description);
extern Dvar_RegisterInt_t Dvar_RegisterInt;

extern cmd_functions_s* cmd_functions;//sp = 17507D8   mp = 0x1D72DD8

extern dvar_t *cg_drawfps;

void OpenMenu(const char* name);

typedef struct
{
	char name[32];
	int index;
	int license;
} dlcmap_t;

// inline cmd functions
extern DWORD* cmd_id;
extern DWORD* cmd_argc;
extern DWORD** cmd_argv;

extern DWORD* cmd_id_sv;
extern DWORD* cmd_argc_sv;
extern DWORD** cmd_argv_sv;

extern DWORD* scr_numParam;

inline int	Cmd_Argc( void )
{
	return cmd_argc[*cmd_id];
}

inline char *Cmd_Argv( int arg )
{
	if ( (unsigned)arg >= cmd_argc[*cmd_id] ) {
		return "";
	}
	return (char*)(cmd_argv[*cmd_id][arg]);	
}

inline int Cmd_Argc_sv( void )
{
	return cmd_argc_sv[*cmd_id_sv];
}

inline char *Cmd_Argv_sv( int arg )
{
	if ( (unsigned)arg >= cmd_argc_sv[*cmd_id_sv] ) {
		return "";
	}
	return (char*)(cmd_argv_sv[*cmd_id_sv][arg]);	
}

void Com_Printf(const char* format, ...);



