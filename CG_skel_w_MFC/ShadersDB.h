#pragma once

struct _s_shader_entry {
	const char	*name;
	const char	*vertex_shader_file_name;
	const char	*fragment_shader_file_name;
	bool		hasTexture;
	const char	**smaplers; // Terminate with empty string
};

typedef struct _s_shader_entry s_shader_entry;

#define SHADER_DB_ENTRY(PREFIX) #PREFIX, #PREFIX"Vshader.glsl", #PREFIX"Fshader.glsl"
#define NO_TEX					false, NULL

#define SHADER_DB \
{ SHADER_DB_ENTRY(Phong, false),	NO_TEX		},	\
{ SHADER_DB_ENTRY(Toon, false),		NO_TEX		}

//{ SHADER_DB_ENTRY(NormalMap, true), { "texMap", "colorMap", "" } }

#define TEXTURE_DB \
"cowUV.png", "giraffe.png", "normalMap.png"



// Database generators -------------------------------------------------------
#define INIT_SHADERS_DB(database) s_shader_entry database[] = { \
SHADER_DB,														\
{ "", "", "", NO_TEX }											\
}

#define INIT_TEXTURES_DB(database) const char *database[] = { TEXTURE_DB, "" }
