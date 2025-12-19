// Vertex shaders
const char* shapeV = "#version 330 core\n"
	"layout (location = 0) in vec2 pos;\n"
	"out vec2 fPos;\n"

	"void main(){\n"
		"fPos = pos;\n"
		"gl_Position = vec4(pos, 1.0, 1.0);}\0";


const char* imageV = "#version 330 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec2 aTexCoord;\n"

	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"
	"void main(){\n"
		"gl_Position = vec4(aPos, 1.0, 1.0);\n"
		"TexCoord = aTexCoord;}\0";

const char* fontV = "#version 330 core\n"
	"layout (location = 0) in vec4 vertex; \n"
	"out vec2 TexCoords;\n"

	"void main(){\n"
	"	gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
	"	TexCoords = vertex.zw;}";


// Fragment shaders		
const char* circleF = "#version 330 core\n"
	"uniform vec4 color;\n"
	"uniform float radius;\n"
	"uniform vec2 pos;\n"
	"uniform float width = 0;\n"
	"vec4 outColor;\n"

	"void main(void){\n"
		"vec2 p = gl_FragCoord.xy;\n"
		"vec2 center = pos + radius/4;\n"
		"float d = length(p - center) - radius/4;\n"
		"float t = clamp(d, 0.0, 1.0);\n"
		"outColor =vec4(color.rgb, color.a-t);\n"

		

		"if (width != 0){\n"
			"float d2 = length(p - center) - (radius-width)/4;\n"
			"float t2 = clamp(d2, 0.0, 1.0);\n"
			"if (t2 == 0)\n"
				"outColor.a = 0;\n"
		"}\n"

		"gl_FragColor = outColor;}";

const char* colorSelectorV = "#version 330 core\n"
	"layout (location = 0) in vec2 pos;\n"

	"void main(){\n"
		"gl_Position = vec4(pos, 1.0, 1.0);}\0";

const char* colorSelectorF = "#version 330 core\n"
	"uniform float hue;\n"
	"uniform vec2 pos;\n"
	"vec4 outColor;\n"
	"uniform vec2 size;\n"

	"void main(void){\n"
		"vec2 p = (gl_FragCoord.xy - pos) / size;\n"
		"vec3 color;\n"
		"float h = hue;\n"
		"float saturation = p.x;\n"
		"float value = p.y;\n"
		"if (saturation <= 0) gl_FragColor = vec4(value, value, value, 1);\n"

		"if (h >= 360) h-=360;\n"
		"h /= 60;\n"
		"int i = int(h);\n"
		"float f = h-float(i);\n"
		"float p1 = value * (1.0 - saturation);\n"
		"float q = value * (1.0 - (saturation*f));\n"
		"float t = value * (1.0 - (saturation * (1.0 - f)));\n"
		"if (i == 0) gl_FragColor = vec4(value, t, p1, 1);\n"
		"else if (i == 1) gl_FragColor = vec4(q, value, p1, 1);\n"
		"else if (i == 2) gl_FragColor = vec4(p1, value, t, 1);\n"
		"else if (i == 3) gl_FragColor = vec4(p1, q, value, 1);\n"
		"else if (i == 4) gl_FragColor = vec4(t, p1, value, 1);\n"
		"else gl_FragColor = vec4(value, p1, q, 1);\n"
	"}";
		
const char* hueSelectorF = "#version 330 core\n"
	"uniform vec4 color;\n"
	"uniform vec2 pos;\n"
	"vec4 outColor;\n"
	"uniform vec2 size;\n"
	"uniform int rotate;\n"

	"void main(void){\n"
		"vec2 p = (gl_FragCoord.xy - pos) / size;\n"
		"if (rotate == 0){\n"
			"if (p.x <= 0.33) outColor = vec4(1-(p.x/0.33),(p.x/0.33),0,1);\n"
			"else if (p.x <= 0.67) outColor = vec4(0,1-((p.x-0.33)/0.33),((p.x-0.33)/0.33),1);\n"
			"else outColor = vec4(((p.x-0.67)/0.33),0,1-((p.x-0.67)/0.33),1);\n"
		"}else{\n"
			"if (p.y <= 0.33) outColor = vec4(1-(p.y/0.33),0,(p.y/0.33),1);\n"
			"else if (p.y <= 0.67) outColor = vec4(0,((p.y-0.33)/0.33), 1-((p.y-0.33)/0.33),1);\n"
			"else outColor = vec4(((p.y-0.67)/0.33),1-((p.y-0.67)/0.33),0,1);\n"
		"}\n"
		"outColor.a = 1;\n"

		"gl_FragColor = outColor;}";


const char* shapeF = "#version 330 core\n"
	"out vec4 shaderColor;\n"

	"uniform vec4 color;\n"

    "void main(){\n"
		"shaderColor = color;}\0";

const char* imageF = "#version 330 core\n"
	"out vec4 FragColor;\n"
  
	"in vec2 TexCoord;\n"

	"uniform sampler2D ourTexture;\n"
	"uniform vec4 color;\n"

	"void main(){\n"
		"vec4 texColor = texture(ourTexture, TexCoord);\n"
		"if (texColor.a < 0.1)\n"
			"discard;\n"
		"if (color.x != 1 || color.y != 1 || color.z != 1 || color.w != 1)\n"
			"texColor = color\n;"
    	"FragColor = texColor;}\0";

const char* fontF = "#version 330 core\n"
	"in vec2 TexCoords;\n"
	"out vec4 color;\n"

	"uniform sampler2D text;\n"
	"uniform vec3 textColor;\n"

	"void main(){\n"
		"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"color = vec4(textColor, 1.0) * sampled;\n}";
