#pragma once

#define SHADER_DB_ENTRY(PREFIX, HAS_TEXTURE) #PREFIX, #PREFIX"Vshader.glsl", #PREFIX"Fshader.glsl", HAS_TEXTURE
#define NO_TEX					NULL

//#define SHADER_DB { SHADER_DB_ENTRY(Toon, false),		NO_TEX		}

#define SHADER_DB \
{ SHADER_DB_ENTRY(Phong, false),	NO_TEX		},				\
{ SHADER_DB_ENTRY(NormalMap, true), "normalMap", "colorMap" },		\
{ SHADER_DB_ENTRY(EnviromentMap, true), "normalMap", "colorMap" },		\
{ SHADER_DB_ENTRY(Toon, false),		NO_TEX		},	\
{ SHADER_DB_ENTRY(Marble, false),		NO_TEX		}

// Last element ("") is terminator.
#define TEXTURE_DB \
"cowUV.png", "giraffe.png", "normalMap.png", "5821-normal.png", "BorntoRule-ColorMap.png", \
"BorntoRule-NormalMap.png", "brick.png", "BubbleGrip-ColorMap.png", "BubbleGrip-NormalMap.png", \
"BubblyBricks-ColorMap.png", "BubblyBricks-ColorMap.png", "CarvedSandstone-ColorMap.png", \
"CarvedSandstone-NormalMap.png", "GraniteWall-ColorMap.png", "GraniteWall-NormalMap.png", \
"tree.png","wallpaper.png" ,"white.png",""



// Database entry defs ---------------------------------------------------
#define MAX_SAMPLERS 4

struct _s_shader_entry {
	const char	*name;
	const char	*vertex_shader_file_name;
	const char	*fragment_shader_file_name;
	bool		hasTexture;
	const char	*smapler0; // maximum supported samplers - 2, max 15 chars per sampler
	const char	*sampler1;
	const char  *sampler2;
	const char	*sampler3;
};

typedef struct _s_shader_entry s_shader_entry;


// Database generators -------------------------------------------------------
#define INIT_SHADERS_DB(database) s_shader_entry database[] = { \
SHADER_DB,														\
{ "", "", "", NO_TEX }}

#define INIT_TEXTURES_DB(database) const char *database[] = { TEXTURE_DB, "" }
