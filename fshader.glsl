#version 150

smooth in vec4 _color;
smooth in vec3 _pos;
smooth in vec3 _normal;

out vec4 outputColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(_normal);
    vec3 lightDir = normalize(lightPos - _pos);
    float diff = max(dot(norm, lightDir), 0.2f);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.3f;
    vec3 viewDir = normalize(viewPos - _pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    outputColor = vec4(ambient+diffuse+specular, 1.0f) * _color;
}