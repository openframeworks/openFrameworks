#version 300 es

// Define precision for floats
#ifdef GL_ES
    precision highp float;
#endif

// Inputs
in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;

// Uniforms
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform float timeValX;
uniform float timeValY;
uniform vec2 mouse;

// Outputs (replaces 'varying' in ES2)
out vec4 colorVarying;
out vec2 texCoordVarying;

// Generate a random value from four points
vec4 rand(vec2 A, vec2 B, vec2 C, vec2 D) { 
    vec2 s = vec2(12.9898, 78.233); 
    vec4 tmp = vec4(dot(A, s), dot(B, s), dot(C, s), dot(D, s)); 
    return fract(sin(tmp) * 43758.5453) * 2.0 - 1.0; 
} 

// Similar to a Perlin noise function
float noise(vec2 coord, float d) { 
    vec2 C[4]; 
    float d1 = 1.0 / d;

    C[0] = floor(coord * d) * d1; 
    C[1] = C[0] + vec2(d1, 0.0); 
    C[2] = C[0] + vec2(d1, d1); 
    C[3] = C[0] + vec2(0.0, d1);

    vec2 p = fract(coord * d); 
    vec2 q = 1.0 - p; 
    vec4 w = vec4(q.x * q.y, p.x * q.y, p.x * p.y, q.x * p.y); 

    return dot(rand(C[0], C[1], C[2], C[3]), w); 
} 

void main() {
    // Get our current vertex position so we can modify it
    vec4 pos = projectionMatrix * modelViewMatrix * position;
    
    // Generate some noise values based on vertex position and the time value
    float noiseAmntX = noise(vec2(-timeValX + pos.x / 1000.0, 100.0), 20.0);
    float noiseAmntY = noise(vec2(timeValY + pos.y / 1000.0, pos.x / 2000.0), 20.0);

    // Generate noise for our blue pixel value
    float noiseB = noise(vec2(timeValY * 0.25, pos.y / 2000.0), 20.0);

    // Calculate the distance between the mouse and the vertex and apply a repelling force
    vec2 d = vec2(pos.x, pos.y) - mouse;
    float len = sqrt(d.x * d.x + d.y * d.y);
    if (len < 300.0 && len > 0.0) {
        // Calculate the distance as a percentage (0-1 range)
        float pct = len / 300.0;
        pct *= pct;  // Apply a curve to the distance
        pct = 1.0 - pct;  // Flip the value so the closer we are, the greater the repulsion

        // Normalize the repulsion vector
        d /= len;

        // Apply the repulsion to our position
        pos.x += d.x * pct * 90.0;
        pos.y += d.y * pct * 90.0;
    }

    // Modify our position with the smooth noise
    pos.x += noiseAmntX * 20.0;
    pos.y += noiseAmntY * 10.0;
    
    // Set the position to be the actual position rendered
    gl_Position = pos;

    // Pass the color and texture coordinate to the fragment shader
    colorVarying = color;
    texCoordVarying = texcoord;
}
