#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VerSh_OUT {
	vec3 verWorldPosition;
	vec3 verNormal;
} fragSh_in[];

out GeomSh_OUT {
	vec3 verWorldPosition;
	vec3 verNormal;
} geomSh_out;


uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude; 
    return position + vec4(direction, 0.0f);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
    //vec3 normal = GetNormal();
    //gl_Position = explode(gl_in[0].gl_Position, normal);

    gl_Position = explode(gl_in[0].gl_Position, fragSh_in[0].verNormal);
	geomSh_out.verWorldPosition = fragSh_in[0].verWorldPosition;
	geomSh_out.verNormal = fragSh_in[0].verNormal;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, fragSh_in[1].verNormal);
	geomSh_out.verWorldPosition = fragSh_in[1].verWorldPosition;
	geomSh_out.verNormal = fragSh_in[1].verNormal;
	EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, fragSh_in[2].verNormal);
	geomSh_out.verWorldPosition = fragSh_in[2].verWorldPosition;
	geomSh_out.verNormal = fragSh_in[2].verNormal;
	EmitVertex();
    EndPrimitive();
}