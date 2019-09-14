#version 430 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 ourNormal;
in vec3 fragPos;
in float visibility;
uniform vec3 lightPos;
uniform vec3 skyColour;
uniform vec3 viewPos;
void main() {
    float ambientStrength = 0.3f;
    float specularStrength = 0.5f;
    vec3 norm = normalize(ourNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0f, 1.0);
    float dist = distance(fragPos, lightPos);
    float diffuse = max(dot(norm, lightDir), 0.0f);
    vec3 result = min((specular*0.7 + (ambientStrength + diffuse) * ourColor), (specular*0.7 + (ambientStrength + diffuse) * ourColor) * 20/ dist / sqrt(dist));
    FragColor = mix(vec4(skyColour, 1.0f),vec4(result, 1.0f), visibility);
}
