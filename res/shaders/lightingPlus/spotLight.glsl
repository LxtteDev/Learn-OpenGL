#version 330 core

struct Material {
    sampler2D specular;
    sampler2D diffuse;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 specular;
    vec3 ambient;
    vec3 diffuse;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;
};

in vec3 sFragPosition;
in vec2 sTexCoord;
in vec3 sNormal;

out vec4 FragColor;

uniform vec3 uViewPosition;

uniform Light light;
uniform Material material;

float calculateIntensity(float distance, float constant, float linear, float quadratic) {
    return 1.0 / ( constant + linear * distance + quadratic * ( distance * distance ) );
}

void main() {
    vec3 normal = normalize(sNormal);
    vec3 lightingDirection = normalize(light.position - sFragPosition);

    vec3 ambient = light.ambient * texture(material.diffuse, sTexCoord).rgb;

    float diff = max(dot(normal, lightingDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, sTexCoord).rgb;

    vec3 viewDirection = normalize(uViewPosition - sFragPosition);
    vec3 reflectDirection = reflect(-lightingDirection, sNormal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, sTexCoord).rgb;

    // Spotlight
    float theta = dot(lightingDirection, normalize(-light.direction));
    float epsilon   = light.cutoff - light.outerCutoff;
    float intensity = clamp(( theta - light.outerCutoff ) / epsilon, 0.0, 1.0); 

    diffuse *= intensity;
    specular *= intensity;

    // Attenuation
    float distance = length(light.position - sFragPosition);
    float attenuation = calculateIntensity(distance, light.constant, light.linear, light.quadratic);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}