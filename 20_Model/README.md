# Modeling

### ASSIMP Structure

<img width="800" height="459" alt="assimp" src="https://github.com/user-attachments/assets/ff71d057-a0dc-4a72-b35e-76c861df8cf9" />

<br/>
<br/>

### fragment shader에서 normal을 normalize 하는 이유
vertex shader에서 normalize를 해도 rasterizer에서 보간할 때 그 값이 단위 벡터라고 보장할 수 없기 때문이다.

$ex)$

$a (1.0, 0.0, 0.0), b (0.0, 1.0, 0.0)$

$c = 0.5 * a + 0.5 * b = (0.5, 0.5, 0.0)$

$|c| != 1$

따라서 fragment shader에서 normalize를 한다.

<br/>
<br/>

### swizzle mask
나는 채널이 1개일 때 다음과 같이 사용했었다.
``` c++
GLenum format;
if (nrChannels == 1) format = GL_RED;
...
```
메모리 절약을 위해 1개의 채널(그레이 채널)을 사용할 때, 다음과 같이 사용하면 GL_RED를 제외한 다른 색은 0.0이 되어 화면이 빨간색으로 나오게 된다.
따라서 쉐이더가 이 값을 읽을 때, GL_GREEN과 GL_BLUE 또한 GL_RED값으로 채워주도록 한다.
``` c++
if (nrChannels == 1) {
    GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
} else {
    GLint swizzleMask[] = {GL_RED, GL_GREEN, GL_BLUE, GL_ONE};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
}
```

<br/>
<br/>

### TBN과 normalMap 사용 방법
``` GLSL
// ...
in vec3 Tangent;
in vec3 Bitangent;
in vec3 Normal;
in vec3 TexCoords;

uniform sampler2D texture_normal1;

void main() {
    // ...

    // make TBN
    vec3 T = normalize(Tangent);
    vec3 B = normalize(Bitangent);
    vec3 N = normalize(Normal);
    mat3 TBN = mat3(T, B, N);

    // make norm
    vec3 norm = texture(texture_normal1, TexCoords).rgb;
    norm = 2.0 * norm - 1.0;
    norm = TBN * norm;
    norm = normalize(norm);

    // ...
}
```
