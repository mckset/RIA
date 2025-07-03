class Shader{
	public:
		unsigned int ID;
		
		void Init(const char* vs, const char* fs){
			unsigned int vertex, fragment;
				
			// Vertex shader 
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vs, NULL);
			glCompileShader(vertex);
			CheckShader(vertex);

			// Fragment Shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fs, NULL);
			glCompileShader(fragment);

			// Shader Program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);

			// delete the shaders as they're linked into our program now and no longer necessary
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		void Use(bool force = false){
			if (ShaderID != ID || force){
				ShaderID = ID;
				glUseProgram(ID);
			}
		}

	private:
		void CheckShader(unsigned int shader){	
			int  success;
			char info[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success){
				glGetShaderInfoLog(shader, 512, NULL, info);
				printf("ERROR loading shaders: %s\n", info);
			}
		}

		void CheckProgram(unsigned int program){
			int  success;
			char info[512];
			glGetProgramiv(program, GL_COMPILE_STATUS, &success);
			if (!success){
				glGetProgramInfoLog(program, 512, NULL, info);
				printf("ERROR loading program: %s\n", info);
			}
		}
};

Shader sImage;
Shader sShape;
Shader sGradient;
Shader sHueSelector;
Shader sColorSelector;
Shader sCircle;
Shader sBox;