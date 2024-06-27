class Shape{
	public:
		void DrawBox(Vector2 start, Vector2 size, Color color, float w = 1.0f, bool fixed = false, bool sort = false){
			Vector2 l1=start, l2=start.Add(size);

			// Moves the first point to the lower left corner
			if (sort){
				if (size.x < 0){
					l1.x += size.x;
					size.x = -size.x;
				}
				if (size.y < 0){
					l1.y += size.y;
					size.y = -size.y;
				}
			}

			Draw(l1, Vector2{size.x, w}, color, fixed); // Lower Left to Lower Right
			Draw(Vector2{l1.x+size.x-w, l1.y+w}, Vector2{w, size.y-w}, color, fixed); // Lower Right to Upper Right
			Draw(Vector2{l1.x, l1.y+size.y-w}, Vector2{size.x-w, w}, color, fixed); // Upper Right to Upper Left
			Draw(l1.Add(0.0f, w), Vector2{w, size.y-w*2}, color, fixed); // Upper Left to Lower Right
		}

		void DrawCircle(Vector2 position, float size, float border, Color color, bool fixed = false){
			sCircle.Use();
			size = size * 2;
			position -= size/8;
			Vector2 shaderPos = position;
			position.x = position.x*2/Width;
			position.y = position.y*2/Height;
			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position.x -= View->x*2/Width;
				position.y -= View->y*2/Height;
				size *= (*Scale);
			}	
			position.x-=1;
			position.y-=1;

			if (!fixed)
				position = position * (*Scale);

			float w = position.x + size/Width;
			float h = position.y + size/Height;

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

			glUniform1f(glGetUniformLocation(ShaderID, "radius"), size/2);
			glUniform1f(glGetUniformLocation(ShaderID, "width"), border);
			glUniform2f(glGetUniformLocation(ShaderID, "pos"), shaderPos.x, shaderPos.y);
			glUniform4f(glGetUniformLocation(ShaderID, "color"), color.r, color.g, color.b, color.a);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
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
			glUniform4f(glGetUniformLocation(ShaderID, "color"), color.r, color.g, color.b, color.a);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void DrawLine(Vector2 position, Vector2 end, Color color, float width=1.0f, bool fixed = true){
			sShape.Use();
			position = position.Multiply(2).Divide(Width, Height);
			end = end.Multiply(2).Divide(Width, Height);
			width=width*2/Width;

			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position = position - ((*View * 2) / Vector2{fWidth, fHeight});
				end = end - ((*View * 2) / Vector2{fWidth, fHeight});
				width *= (*Scale);
			}	
			position = position.Subtract(1);
			end = end.Subtract(1);

			if (!fixed){
				position = position.Multiply((*Scale));
				end = end.Multiply((*Scale));
			}

			// Location of the object
			float data[] = {  
				end.x,  end.y, // Top Left
				position.x, position.y, // Bottom left
				position.x+width, position.y+width,	
				end.x+width,  end.y+width,
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
			glUniform4f(glGetUniformLocation(ShaderID, "color"), color.r, color.g, color.b, color.a);
					
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

		void Draw(Vector2 position, Vector2 size, Color color, bool fixed = false){
			sShape.Use();
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
			glUniform4f(glGetUniformLocation(ShaderID, "color"), color.r, color.g, color.b, color.a);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	private:
		Vector2 Rotate(Vector2 position, Vector2 center, float angle){
			float rX = cos(angle) * (position.x - center.x) - sin(angle) * (position.y-center.y) + center.x;
			float rY = sin(angle) * (position.x - center.x) + cos(angle) * (position.y - center.y) + center.y;

			/*
			Vector2 output;
			
			output.x = ((position.x - origin.x) * cos(angle))
				-((position.y - origin.y) * sin(angle));
			output.y = ((position.x - origin.x) * sin(angle))
				+ ((position.y - origin.y) * cos(angle));
			output = output.Add(origin);
			float newA = atan(output.x/output.y)*(float)DEGREES;	
			*/
			return Vector2{rX,rY};
		}
};
Shape shape;