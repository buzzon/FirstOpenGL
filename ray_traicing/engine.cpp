#include "engine.h"

void engine::initGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("ray traicing");

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
}

void engine::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);								// „истим экран

	generateTexture();											// √енерируютекстуру

	glBegin(GL_QUADS);											// –исую пр€моуголькик заполн€ющий весь экран
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);	// к нему примен€ю новоиспеченную текстуру
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);		//
	glEnd();													//

	glFlush();													// ”буждаемс€ что буфер выведен
}

void engine::generateTexture()
{
	GLuint texName;
	static unsigned char framebuffer[height][width][3];

	build_and_render(framebuffer);     // –ендерю картинку и записываю еЄ в framebuffer

	glGenTextures(1, &texName);																		  // √енерирую текстуру и назначаю еЄ активной
	glBindTexture(GL_TEXTURE_2D, texName);															  //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);								  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);								  // 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);  //
}

vec3f engine::cast_ray(const vec3f& origin, const vec3f norm_direction, const std::vector<IObject3d*> &composition)
{
	vec3f point, N;
	material material;

	if (!scene_intersect(origin, norm_direction, composition, point, N, material)) {
		return vec3f(0.2, 0.2, 0.2); //get_background_color();
	}
	return material.diffuse_color;
}

bool engine::scene_intersect(const vec3f& origin, const vec3f& norm_direction, const std::vector<IObject3d*>& composition, vec3f& hit, vec3f& N, material& material)
{
	float spheres_dist = std::numeric_limits<float>::max();

	for (size_t i = 0; i < composition.size(); i++) {
		float dist_i = 0;
		if (composition[i]->intersect(origin, norm_direction, dist_i)) {
			if (dist_i < spheres_dist)
			{
				spheres_dist = dist_i;
				hit = origin + norm_direction * dist_i;
				//N = composition[i].get_normal(hit);
				material = composition[i]->get_material();
			}

		}
	}
	return spheres_dist < 1000;
}

void engine::build_and_render(GLubyte(&framebuffer)[height][width][3])
{
	material      ivory(vec3f(0.4, 0.4, 0.3));
	material red_rubber(vec3f(0.3, 0.1, 0.1));

	std::vector<IObject3d*> composition;
	composition.push_back(&sphere(vec3f(-3,    0,   -16), 2, ivory));	
	composition.push_back(&sphere(vec3f(-1.0, -1.5, -12), 2, red_rubber));
	composition.push_back(&sphere(vec3f( 1.5, -0.5, -18), 3, red_rubber));
	composition.push_back(&sphere(vec3f(7,	   5,   -18), 4, ivory));
	render(composition, framebuffer);
}

void engine::render(const std::vector<IObject3d*>& composition, GLubyte(&framebuffer)[600][800][3])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float fov = 1;
			float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
			float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
			vec3f dir = vec3f(x, y, -1).normalize();

			vec3f color = cast_ray(vec3f(0, 0, 0), dir, composition);

			framebuffer[i][j][0] = color[0] * 255;
			framebuffer[i][j][1] = color[1] * 255;
			framebuffer[i][j][2] = color[2] * 255;
		}
	}
}