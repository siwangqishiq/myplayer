precision highp float;

in vec2 vUv;

uniform sampler2D texY;
uniform sampler2D texU;
uniform sampler2D texV;

out vec4 fragColor;

void main(){
    vec2 uv = vec2(vUv.x , 1.0f - vUv.y);

    float colorY = texture(texY , uv).r;
    float colorU = texture(texU , uv).r - 0.5f;
    float colorV = texture(texV , uv).r - 0.5f;

    vec3 rgbColor = vec3(colorY + 1.14f * colorV,
                    colorY - 0.395f * colorU  - 0.581f * colorV,
                    colorY + 2.032f * colorU);
    fragColor = vec4(rgbColor.bgr, 1.0f);
}