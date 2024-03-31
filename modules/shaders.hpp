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


// Fragment shaders		
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
