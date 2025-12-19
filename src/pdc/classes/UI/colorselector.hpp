Shader sGradient;
Shader sHueSelector;
Shader sColorSelector;

class ColorSelector{
	public:
		Color color;

		Vector2 position;
		Vector2 size;
		Vector2 pickerPos = {1, 1};

		Scrollbar hueBar = Scrollbar{Transparent, White, SCROLLBAR_VERTICAL, 8, 360};
 
		bool Hover(){return mouse.position.Within(position, size);}
		void Reset(){pickerPos = {0,0};}

		void SetColor(Color c){
			Color HSV = c.ToHSV();
			pickerPos.x = HSV.g/100.0f * size.x;
			pickerPos.y = HSV.b/100.0f * size.y;
			hueBar.scroll = HSV.r;
		}

		Color Draw(Vector2 pos, Vector2 s, float barSize, Color baseColor, bool vertical = COLOR_SELECTOR_VERTICAL, bool fixed = POSITION_FIXED){
			position = pos;
			size = s;

			if (mouse.state == LM_DOWN && !hueBar.scrollLock && mouse.position.Within(pos, s))pickerPos = mouse.position-pos;

			Color HSV = baseColor.ToHSV();

			if (!hueBar.scrollLock){
				hueBar.scroll = HSV.r;
				color = ToRGB(HSV.r, ((pickerPos.x) / size.x)*100, ((pickerPos.y) / size.y)*100);
			}else
				color = ToRGB(hueBar.scroll, ((pickerPos.x) / size.x)*100, ((pickerPos.y) / size.y)*100);		

			DrawColorSelector(position, size, ToRGB(HSV.r, 100, 100), fixed);
			shape.DrawCircle(position + pickerPos, COLOR_SELECTOR_PICKER_DIAMETER, COLOR_SELECTOR_PICKER_DIAMETER, color, fixed);
			shape.DrawCircle(position + pickerPos, COLOR_SELECTOR_PICKER_DIAMETER, COLOR_SELECTOR_PICKER_BORDER_SIZE, Black, fixed);

			DrawHueSelector(position + Vector2{COLOR_SELECTOR_PADDING + size.x}, {!vertical ? size.x : barSize, vertical ? size.y : barSize}, vertical, fixed);
			hueBar.Draw({position + Vector2{COLOR_SELECTOR_PADDING + size.x}}, {!vertical ? size.x : barSize, vertical ? size.y : barSize});

			return color;
		}

		void DrawHueSelector(Vector2 position, Vector2 size, bool rotate = SCROLLBAR_VERTICAL, bool fixed = POSITION_FIXED){
			sHueSelector.Use();
			Vector2 shaderSize = size;
			Vector2 shaderPos = position;
			size = size * 2;
			position.x = position.x*2/Width;
			position.y = position.y*2/Height;
			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position.x -= View->x*2/Width;
				position.y -= View->y*2/Height;
				size.x *= (*Scale);
				size.y *= (*Scale);
			}	
			position.x-=1;
			position.y-=1;

			if (!fixed)
				position = position * (*Scale);

			float w = position.x + size.x/Width;
			float h = position.y + size.y/Height;

			// Location of the object
			float data[] = {  
				w,  h,
				w, position.y,
				position.x, position.y,	
				position.x,  h,
			};

			// Indices used to draw a triangle
			int box[] = {
				0, 1, 3,
				1, 2, 3};
				
			// Bind array
			glBindVertexArray(vertexArray);

			// Bind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			// Send vertices to buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			// Bind element buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

			// Send vertices to buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), box, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//
			// Shader options
			//
			glUniform1i(glGetUniformLocation(ShaderID, "rotate"), rotate);
			glUniform2f(glGetUniformLocation(ShaderID, "size"), shaderSize.x, shaderSize.y);
			glUniform2f(glGetUniformLocation(ShaderID, "pos"), shaderPos.x, shaderPos.y);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		
		void DrawColorSelector(Vector2 position, Vector2 size, Color color, bool fixed = true){
			sColorSelector.Use();
			Vector2 shaderSize = size;
			Vector2 shaderPos = position;
			size = size * 2;
			position.x = position.x*2/Width;
			position.y = position.y*2/Height;
			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position.x -= View->x*2/Width;
				position.y -= View->y*2/Height;
				size.x *= (*Scale);
				size.y *= (*Scale);
			}	
			position.x-=1;
			position.y-=1;

			if (!fixed)
				position = position * (*Scale);

			float w = position.x + size.x/Width;
			float h = position.y + size.y/Height;

			// Location of the object
			float data[] = {  
				w,  h,
				w, position.y,
				position.x, position.y,	
				position.x,  h,
			};

			// Indices used to draw a triangle
			int box[] = {
				0, 1, 3,
				1, 2, 3};
				
			// Bind array
			glBindVertexArray(vertexArray);

			// Bind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			// Send vertices to buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			// Bind element buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

			// Send vertices to buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), box, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//
			// Shader options
			//

			// Color
			glUniform2f(glGetUniformLocation(ShaderID, "size"), shaderSize.x, shaderSize.y);
			glUniform2f(glGetUniformLocation(ShaderID, "pos"), shaderPos.x, shaderPos.y);
			glUniform1f(glGetUniformLocation(ShaderID, "hue"), color.ToHSV().r);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void DrawGradient(Vector2 position, Vector2 size, Color color1, Color color2, bool fixed = false){
			sGradient.Use();
			size = size * 2;
			position.x = position.x*2/Width;
			position.y = position.y*2/Height;
			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position.x -= View->x*2/Width;
				position.y -= View->y*2/Height;
				size.x *= (*Scale);
				size.y *= (*Scale);
			}	
			position.x-=1;
			position.y-=1;
			if (!fixed)
				position = position * (*Scale);
			float w = position.x + size.x/Width;
			float h = position.y + size.y/Height;
	
			// Location of the object
			float data[] = {  
				w,  h,
				w, position.y,
				position.x, position.y,	
				position.x,  h,
			};

			// Indices used to draw a box
			int box[] = {
				0, 1, 3,
				1, 2, 3};

				
			// Bind array
			glBindVertexArray(vertexArray);

			// Bind vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			// Send vertices to buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

			// Bind element buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

			// Send vertices to buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), box, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//
			// Shader options
			//

			// Color
			glUniform4f(glGetUniformLocation(ShaderID, "color1"), color1.r, color1.g, color1.b, color1.a);
			glUniform4f(glGetUniformLocation(ShaderID, "color2"), color2.r, color2.g, color2.b, color2.a);

			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
};