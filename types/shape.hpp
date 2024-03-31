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

		void DrawLine(Vector2 position, Vector2 end, Color color, float width=1.0f, bool fixed = true){
			position = position.Multiply(2).Divide(Width, Height);
			end = end.Multiply(2).Divide(Width, Height);
			width=width*2/Width;

			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position = position.Subtract(View.Multiply(2).Divide(Width, Height));
				end = end.Subtract(View.Multiply(2).Divide(Width, Height));
				width *= Scale;
			}	
			position = position.Subtract(1);
			end = end.Subtract(1);

			if (!fixed){
				position = position.Multiply(Scale);
				end = end.Multiply(Scale);
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
			glUniform4f(glGetUniformLocation(sShape.ID, "color"), color.r, color.g, color.b, color.a);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void Draw(Vector2 position, Vector2 size, Color color, bool fixed = false){
			size = size.Multiply(2);
			position.x = position.x*2/Width;
			position.y = position.y*2/Height;
			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				position.x -= View.x*2/Width;
				position.y -= View.y*2/Height;
				size.x *= Scale;
				size.y *= Scale;
			}	
			position.x-=1;
			position.y-=1;

			if (!fixed)
				position = position.Multiply(Scale);

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
			glUniform4f(glGetUniformLocation(sShape.ID, "color"), color.r, color.g, color.b, color.a);
				
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			glBindVertexArray(vertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	private:
		Vector2 Rotate(Vector2 position, Vector2 origin, float angle){
			Vector2 output;
			output.x = ((position.x - origin.x) * cos(angle))
				-((position.y - origin.y) * sin(angle));
			output.y = ((position.x - origin.x) * sin(angle))
				+ ((position.y - origin.y) * cos(angle));
			output = output.Add(origin);
			return output;
		}
};
Shape shape;
