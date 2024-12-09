#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;

out vec3 normalIn;
out vec3 worldPosition;

uniform mat4 objMatrix;
uniform mat4 cameraMatrix;

uniform float glfwTime; // Time for animation
uniform float material_waveHeight;
uniform float material_waveSpeed;
uniform float material_waveFrequency;

const vec2 directions[4] = vec2[](vec2(0.25, 0.75), vec2(0.5, 0.5), vec2(1, 0), vec2(0, 1));

void main() {
    // Transform vertex position to world space
    worldPosition = vec3(objMatrix * vec4(aPos, 1.0));

    // Compute wave displacement
    float waveSum = 0.0;
    vec3 waveNormal = vec3(0.0, 1.0, 0.0); // Base normal vector

    for (int i = 0; i < 4; i++) {
        vec2 direction = normalize(directions[i]);
        float phase = float(i) * 3.14 / 2.0; // Different phase for each wave
        float wave = sin(dot(worldPosition.xz, direction) * material_waveFrequency + glfwTime * material_waveSpeed + phase);
        // Add weighted contribution of this wave
        waveSum += wave * material_waveHeight / float(i + 1);
        // Accumulate normals for lighting (gradient approximation)
        waveNormal += vec3(-direction.x * wave, 1.0, -direction.y * wave) * material_waveHeight;
    }

    // Apply wave displacement to the vertex's y-coordinate
    worldPosition.y += waveSum;

    // Recalculate normal based on wave slope
    normalIn = normalize(mat3(transpose(inverse(objMatrix))) * normalize(waveNormal));

    // Transform to clip space for rendering
    gl_Position = cameraMatrix * vec4(worldPosition, 1.0);
}