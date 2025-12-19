Shader sShape;
Shader sCircle;
Shader sBox;

class Shape{
	public:
		void DrawBox(Vector2 position, Vector2 size, Color color, float width = 1.0f, bool fixed = false, float angle = 0.0f){
			Draw(position, {width, size.y}, color, fixed, angle, position + size/2);
			Draw(position + Vector2{size.x, 0}, {width, size.y}, color, fixed, angle, position + size/2);
			Draw(position, {size.x, width}, color, fixed, angle, position + size/2);
			Draw(position + Vector2{0, size.y}, {size.x+width, width}, color, fixed, angle, position + size/2);
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

			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void DrawLine(Vector2 start, Vector2 end, Color color, float width=1.0f, bool fixed = false){

			if (start.x == end.x){
				Draw(start-Vector2{width/2, 0}, {width, end.y - start.y}, color, fixed, 0, start);
				return;
			}else if (start.y == end.y){
				Draw(start-Vector2{0, width/2}, {end.x - start.x, width}, color, fixed, 0, start);
				return;
			}

			int flip = 1;

			if (start.x > end.x && start.y > end.y){
				float e = end.x;
				end.x = start.x;
				start.x = e;
				e = end.y;
				end.y = start.y;
				start.y = e;
			}

			else if (start.y > end.y || start.x > end.x){
				flip = -1;
				if (start.x > end.x){
					float e = end.x;
					end.x = start.x;
					start.x = e;
					e = end.y;
					end.y = start.y;
					start.y = e;
				}
			}
			
			Vector2 position = start;
			float h = sqrt(pow((end.x - start.x),2) + pow((end.y - start.y),2));
			Vector2 size = {h, width};
			Vector2 s = {end.x, start.y};
			float a = s.Angle(position, end);
			Draw(position, size, color, fixed, a*flip, position);
			return;
			sShape.Use();
			
			start *= Vector2{2/fWidth, 2/fHeight};
			end *= Vector2{2/fWidth, 2/fHeight};
			// Multiplied by 2 to offset properly otherwise it will only move half
			if (!fixed){
				start -= *View*2/Vector2 {fWidth, fHeight};
				end -= *View*2/Vector2 {fWidth, fHeight};
			}	

			start -= 1;
			end -= 1;

			if (!fixed){
				start *= *Scale;
				end *= *Scale;
			}

			width *= 2.0f/fWidth;

			// Location of the object
			float data[] = {  
				end.x-width/2,  end.y-width/2, // Top Left
				start.x-width/2, start.y-width/2, // Bottom left
				start.x+width/2, start.y+width/2,	
				end.x+width/2,  end.y+width/2,
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

		void Draw(Vector2 position, Vector2 size, Color color, bool fixed = false, float angle = 0.0f, Vector2 origin = {infinity}){
			sShape.Use();

			
			Vector2 tR = Vector2{position.x + size.x, position.y + size.y};
			Vector2 tL = Vector2{position.x, position.y + size.y};
			Vector2 bR = Vector2{position.x + size.x, position.y};
			Vector2 bL = Vector2{position.x, position.y};

			if (angle != 0){
				if (angle < 0)
					angle += 360;
				else if (angle > 360*RADIANS)
					angle -= 360;
				float a = angle*RADIANS;
				Vector2 center = origin.x == infinity ? position + size/2 : origin;
				tR.Rotate(center, a);
				tL.Rotate(center, a);
				bR.Rotate(center, a);
				bL.Rotate(center, a);
			}

			// Converts pixels to screen space
			if (!fixed){
				tR = (Vector2{tR.x*2/fWidth-1,tR.y*2/fHeight-1} - ((*View) * 2 / Vector2{fWidth, fHeight})) * (*Scale);
				tL = (Vector2{tL.x*2/fWidth-1,tL.y*2/fHeight-1} - ((*View) * 2 / Vector2{fWidth, fHeight})) * (*Scale);
				bR = (Vector2{bR.x*2/fWidth-1,bR.y*2/fHeight-1} - ((*View) * 2 / Vector2{fWidth, fHeight})) * (*Scale);
				bL = (Vector2{bL.x*2/fWidth-1,bL.y*2/fHeight-1} - ((*View) * 2 / Vector2{fWidth, fHeight})) * (*Scale);
					
			}else{
				// Multiplied by 2 to offset properly otherwise it will only move half
				tR = Vector2{tR.x*2/fWidth-1,tR.y*2/fHeight-1};
				tL = Vector2{tL.x*2/fWidth-1,tL.y*2/fHeight-1};
				bR = Vector2{bR.x*2/fWidth-1,bR.y*2/fHeight-1};
				bL = Vector2{bL.x*2/fWidth-1,bL.y*2/fHeight-1};
			}

			if (!fixed)
				position = position * (*Scale);

			// Location of the object
			float data[] = {
				// positions 
				tR.x, tR.y,			 // top right
				bR.x, bR.y,  		// bottom right
				bL.x, bL.y,  		 // bottom left
				tL.x, tL.y 		// top left 
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

			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	private:
		Vector2 Rotate(Vector2 position, Vector2 center, float angle){
			float rX = cos(angle) * (position.x - center.x) - sin(angle) * (position.y-center.y) + center.x;
			float rY = sin(angle) * (position.x - center.x) + cos(angle) * (position.y - center.y) + center.y;
			return Vector2{rX,rY};
		}
};
Shape shape;