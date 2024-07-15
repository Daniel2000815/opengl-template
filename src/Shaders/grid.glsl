#version 300 es

precision highp float;
precision highp sampler2D;

float subdivisions = 7.0f;
vec3 background_color = vec3(1.0f);
vec3 background_color_two = vec3(0.25f);
vec3 grid_color = vec3(1.0f, 0.0f, 0.0f);

float line_width = 0.1f;
vec3 line_color = vec3(0.5f, 0.5f, 0.9f);
float line_scale_x = 1.0f;
float line_scale_y = 1.0f;


// normalized coordinates, (0,0) is the bottom left
in vec2 uv;
out vec4 out_color;

float grid(vec2 uv, vec2 lineWidth, bool check) {
    vec4 uvDDXY = vec4(dFdx(uv), dFdy(uv));
    vec2 uvDeriv = vec2(length(uvDDXY.xz), length(uvDDXY.yw));
    
    bvec2 invertLine = bvec2(lineWidth.x > 0.5, lineWidth.y > 0.5);
    vec2 targetWidth = vec2(
        invertLine.x ? 1.0 - lineWidth.x : lineWidth.x,
        invertLine.y ? 1.0 - lineWidth.y : lineWidth.y
    );
    
    vec2 lineAA = uvDeriv * 1.5;
    vec2 gridUV = abs((fract(uv)) * 2.0 - 1.0);
    
    if (check) {
        targetWidth = vec2(1.0);
        lineAA *= 1.5;
        
        float index = 0.0;
        index += step(1.0, mod(uv.x, 2.0));
        index += step(1.0, mod(uv.y, 2.0)) * 2.0;
        
        if (index == 1.0 || index == 2.0) {
            lineAA = -lineAA;
        }
    } else {
        gridUV = vec2(
            invertLine.x ? gridUV.x : 1.0 - gridUV.x,
            invertLine.y ? gridUV.y : 1.0 - gridUV.y 
        );
    }
    
    vec2 drawWidth = clamp(targetWidth, uvDeriv, vec2(1.0));
    vec2 gridBase = smoothstep(drawWidth + lineAA, drawWidth - lineAA, gridUV);
    
    gridBase *= clamp(targetWidth / drawWidth, 0.0, 1.0);
    gridBase = mix(gridBase, targetWidth, clamp((uvDeriv * 2.0 - 1.0), 0.0, 1.0));
    gridBase.x = invertLine.x ? 1.0 - gridBase.x : gridBase.x;
    gridBase.y = invertLine.y ? 1.0 - gridBase.y : gridBase.y;
    
    return mix(gridBase.x, 1.0, gridBase.y);
}

float dashed_grid(vec2 uv, vec2 lineWidth, vec2 dot_width, vec2 count, float add) {
    lineWidth = 1.0 - lineWidth;
    float gridX = 1.0 - grid(vec2(uv.x * count.x, uv.y + add), vec2(dot_width.x, lineWidth.x), false);
    float gridY = 1.0 - grid(vec2(uv.y * count.y, uv.x + add), vec2(dot_width.y, lineWidth.y), false);
    
    float result = mix(gridX, 1.0, gridY);
    return result;
}

// https://godotshaders.com/shader/dashed-grid-the-best-darn-grid-shader-yet/
void main(){
    vec2 st = uv * subdivisions;
    
    float checkeredGrid = grid(st, vec2(0.5), true);

    float pointGrid = dashed_grid(
        st + 0.5, vec2(line_width),
        1.0 - vec2(line_scale_x, line_scale_y),
        vec2(1.0), 0.0
    );
    
    vec3 gridColor = mix(background_color, background_color_two, checkeredGrid);
    //if (mainGrid != 0.0) gridColor = mix(gridColor, grid_color, mainGrid);
    if (pointGrid != 0.0) gridColor = mix(gridColor, line_color, pointGrid);
    
    out_color = vec4(gridColor, 1.0);
}