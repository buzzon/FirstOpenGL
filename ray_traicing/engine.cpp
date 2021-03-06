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
	glClear(GL_COLOR_BUFFER_BIT);								// ������ �����

	generateTexture();											// �����������������

	glBegin(GL_QUADS);											// ����� ������������� ����������� ���� �����
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);	// � ���� �������� �������������� ��������
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);		//
	glEnd();													//

	glFlush();													// ���������� ��� ����� �������
}

void engine::generateTexture()
{
	GLuint texName;
	static unsigned char framebuffer[height][width][3];

	build_and_render(framebuffer);     // ������� �������� � ��������� � � framebuffer

	glGenTextures(1, &texName);																		  // ��������� �������� � �������� � ��������
	glBindTexture(GL_TEXTURE_2D, texName);															  //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);								  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);								  // 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);  //
}

vec3f engine::cast_ray(const vec3f& origin, const vec3f norm_direction, const std::vector<IObject3d*> &composition, const std::vector<light>& lights, size_t depth)
{
	vec3f point, normal;
	Material material;

	// ����
	if (depth > recursion_depth || !scene_intersect(origin, norm_direction, composition, point, normal, material)) {
		return vec3f(0.2, 0.2, 0.2); //get_background_color();
	}

	// ���������
	vec3f reflect_dir = reflect(norm_direction, normal);
	vec3f reflect_orig = reflect_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3;
	vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, composition, lights, depth + 1);

	
	float diffuse_light_intensity = 0;
	float specular_light_intensity = 0;

	for (size_t i = 0; i < lights.size(); i++) {
		vec3f light_dir = (lights[i].position - point).normalize();
		float light_distance = (lights[i].position - point).norm();

		// ����
		vec3f shadow_orig = light_dir * normal < 0 ? point - normal * 1e-3 : point + normal * 1e-3; // ������� ����� ������ ���� � �������, ����� ����������� ����� ������
		vec3f shadow_pt, shadow_N;
		Material tmpmaterial;
		// ����������, �� ���������� �� ��� �����-�������� ����� ������� ����� ����� ����� �������� ������� ��������� ��� ����� ��������� �����
		if (scene_intersect(shadow_orig, light_dir, composition, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt - shadow_orig).norm() < light_distance)
			continue;

		// ��������� ��������
		diffuse_light_intensity += lights[i].intensity * std::max(0.f, light_dir * normal);
		// �����
		specular_light_intensity += powf(std::max(0.f, reflect(light_dir, normal) * norm_direction), material.specular_exponent) * lights[i].intensity;
	}

	return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + vec3f(1, 1, 1) * specular_light_intensity * material.albedo[1] + reflect_color * material.albedo[2];
}

bool engine::scene_intersect(const vec3f& origin, const vec3f& norm_direction, const std::vector<IObject3d*>& composition, vec3f& hit, vec3f& N, Material& material)
{
	float spheres_dist = std::numeric_limits<float>::max();

	for (size_t i = 0; i < composition.size(); i++) {
		float dist_i = 0;
		if (composition[i]->intersect(origin, norm_direction, dist_i)) {
			if (dist_i < spheres_dist)
			{
				spheres_dist = dist_i;
				hit = origin + norm_direction * dist_i;
				N = composition[i]->get_normal(hit);
				material = composition[i]->get_material();
			}
		}
	}
	return spheres_dist < 1000;
}

vec3f engine::reflect(const vec3f& dir, const vec3f& normal)
{
	return dir - normal * 2.f * (dir * normal);
}

void engine::build_and_render(GLubyte(&framebuffer)[height][width][3])
{
	Material      ivory(vec3f(0.5, 1.0, 0.0), vec3f(0.48, 0.02, 0.66), 50.);
	Material red_rubber(vec3f(0.9, 0.1, 0.0), vec3f(0.02, 0.10, 0.44), 10.);
	Material     orange(vec3f(0.9, 0.5, 0.0), vec3f(0.99, 0.70, 0.00), 50.);
	Material     mirror(vec3f(0.0, 10.0, 0.8), vec3f(1.0, 1.00, 1.00), 1425.);

	std::vector<IObject3d*> composition;
	composition.push_back(&sphere(vec3f(-3,    0,   -16), 2,      ivory));
	composition.push_back(&sphere(vec3f(-1.0, -1.5, -12), 2,     orange));
	composition.push_back(&sphere(vec3f( 1.5, -0.5, -18), 3, red_rubber));
	composition.push_back(&sphere(vec3f( 7,    5,   -18), 4,     mirror));


	std::vector<light> lights;
	lights.push_back(light(vec3f(-20, 20, 20), 1.5));
	lights.push_back(light(vec3f(30, 50, -25), 1.8));
	lights.push_back(light(vec3f(30, 20, 30),  1.7));

	render(composition, lights, framebuffer);
}

void engine::render(const std::vector<IObject3d*>& composition, const std::vector<light>& lights, GLubyte(&framebuffer)[600][800][3])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float fov = 1.5;
			float x =  (2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.) * height / (float)width;
			float y = -(2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.);
			vec3f dir = vec3f(x, y, -1).normalize();

			vec3f color = cast_ray(vec3f(0, 0, 0), dir, composition, lights);
			float max = std::max(color[0], std::max(color[1], color[2]));
			if (max > 1) color = color * (1. / max);	// ����������� ���������

			framebuffer[i][j][0] = color[0] * 255;
			framebuffer[i][j][1] = color[1] * 255;
			framebuffer[i][j][2] = color[2] * 255;
		}
	}
}