uniform float threshold;

void main()
{
    vec4 pixel = gl_Color;
    if (pixel.r <= threshold){
        pixel.r = 255.f;
        pixel.g = 255.f;
        pixel.b = 255.f;
        pixel.a = 255.f;
    }
    else{
        discard;
    }

    gl_FragColor = pixel;
}
