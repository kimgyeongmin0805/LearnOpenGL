# Model Loading

### Directional Light

<img width="515" height="322" alt="light_casters_directional" src="https://github.com/user-attachments/assets/cb99abd9-6af0-4fa5-94c6-5b61df58d3d6" />


```
struct DirLight {
    vec3 direction;

    // color
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```



### Point Light

<img width="591" height="322" alt="light_casters_point" src="https://github.com/user-attachments/assets/93bc81b0-d012-4621-8e23-2255aa838645" />


```
struct PointLight {
    vec3 position;

    // attenuation
    float constant;
    float linear;
    float quadratic;

    // color
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```
<img width="791" height="119" alt="attenuation_equation" src="https://github.com/user-attachments/assets/42db3b1b-9f5b-4472-92ae-8c9952604218" />
<img width="955" height="560" alt="attenuation_table" src="https://github.com/user-attachments/assets/5c37683d-b13a-4f81-9cfe-3d9602938e07" />



### Spot Light

<img width="447" height="322" alt="light_casters_spotlight_angles" src="https://github.com/user-attachments/assets/703574d8-26c0-4d9d-ac6f-332db7bcb6da" />

```
struct SpotLight {
    vec3 position;
    vec3 direction;
    float inner_cutoff;
    float outer_cutoff;

    // attenuation
    float constant;
    float linear;
    float quadratic;

    // color
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```

<img width="914" height="145" alt="cutoff_equation" src="https://github.com/user-attachments/assets/c55744fe-7905-43f2-9deb-17eb0657a1a8" />
<img width="969" height="471" alt="cutoff_table" src="https://github.com/user-attachments/assets/00133061-4d6d-490f-909d-1af5c3411c69" />

