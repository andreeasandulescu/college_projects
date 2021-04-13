#version 330

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int num_levels;

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 fst_light_position;
uniform vec3 snd_light_position;
uniform vec3 thrd_light_position;
uniform vec3 fth_light_position;
uniform vec3 eye_position;

//material properties:
uniform float material_ka;
uniform float material_ks;
uniform int material_shininess;

layout(location = 0) out vec4 out_color;

void main()
{

	float fat, dist, aux;
	float ambient_light, diffuse_light, specular_light;
	vec3 L,V,H, kd, ks, aux_kd, aux_ks;
	vec4 fst, snd, thrd, fth;

	kd = texture2D(texture_1, texcoord).xyz;
	ks = texture2D(texture_2, texcoord).xyz;

	float material_kd = material_ka;
	ambient_light = material_ka * 0.25;

	//first light
	L = normalize( fst_light_position - world_position );
	V = normalize( eye_position - world_position );
	H = normalize( L + V );
 
	aux = material_kd  * max ( dot(world_normal ,L), 0);
	diffuse_light = floor(aux * num_levels) / num_levels;
	specular_light = 0;
	if (diffuse_light > 0)
	{
		aux = material_ks * pow( max(dot(world_normal, H) ,0), material_shininess);
		specular_light = floor(aux * num_levels) / num_levels;
	}

	dist = distance(world_position , fst_light_position);
	fat = 0.5 * 1/(pow(dist,2) * 0.2);
	fst = vec4( ((diffuse_light + material_ka) * kd + specular_light * ks) * fat  * vec3(0.2,1,0.2), 1.0f);

	//second light
	L = normalize( snd_light_position - world_position );
	H = normalize( L + V );

	aux = material_kd  * max ( dot(world_normal ,L), 0);
	diffuse_light = floor(aux * num_levels) / num_levels;
	specular_light = 0;
	if (diffuse_light > 0)
	{
		aux = material_ks * pow( max(dot(world_normal, H) ,0), material_shininess);
		specular_light = floor(aux * num_levels) / num_levels;
	}

	dist = distance(world_position , snd_light_position);
	fat = 1/(pow(dist,2) * 0.2);
	snd = vec4( ((diffuse_light + material_ka) * kd + specular_light * ks) * fat  * vec3(1,0.2,0.2), 1.0f);

	//third light
	L = normalize( thrd_light_position - world_position );
	H = normalize( L + V );

	aux = material_kd  * max ( dot(world_normal ,L), 0);
	diffuse_light = floor(aux * num_levels) / num_levels;
	specular_light = 0;
	if (diffuse_light > 0)
	{
		aux = material_ks * pow( max(dot(world_normal, H) ,0), material_shininess);
		specular_light = floor(aux * num_levels) / num_levels;
	}

	dist = distance(world_position , thrd_light_position);
	fat = 1/(pow(dist,2) * 0.2);
	thrd = vec4( ((diffuse_light + material_ka) * kd + specular_light * ks) * fat  *vec3(0.2,0.2,1), 1.0f);

	//fourth light
	L = normalize( thrd_light_position - world_position );
	H = normalize( L + V );

	aux = material_kd  * max ( dot(world_normal ,L), 0);
	diffuse_light = floor(aux * num_levels) / num_levels;
	specular_light = 0;
	if (diffuse_light > 0)
	{
		aux = material_ks * pow( max(dot(world_normal, H) ,0), material_shininess);
		specular_light = floor(aux * num_levels) / num_levels;
	}

	dist = distance(world_position , fth_light_position);
	fat = 1/(pow(dist,2) * 0.2);
	fth = vec4( ((diffuse_light + material_ka) * kd + specular_light * ks) * fat * 0.2 *vec3(1,1,1), 1.0f);

	out_color = fst + snd + thrd + fth;
}