#version 300 es
precision mediump float;

in vec4 vfColor;
uniform vec2 koordinate;
uniform int gommemode;
out vec4 fColor;

void main()
{
    //fColor = vec4(1.0, 0.0, 0.0, 1.0); //Aufgabe 1: rot
    /*
    float y = 0.30 * vfColor.r + 0.59 * vfColor.g + 0.11 * vfColor.b;
    fColor = vec4(y, y, y, 1); //Aufgabe 2: graubild 
    */
    /*
    float y = vfColor.b;
    fColor = vec4(y, y, y, 1.0); //Aufgabe 3: grau mit blau
    */
    //fColor = vec4(vfColor.b, vfColor.r, vfColor.g, 1.0); // Aufgabe 4: tauschen
    /*
    float r = gl_FragCoord.x / 512.0;
    float g = gl_FragCoord.y / 512.0;
    fColor = vec4(r, g, 0.0, 1.0); //Aufgabe 5: rot und grün von x und y, am besten CanvasSize aus main.js übergeben
    */
    /*
    float r = 0.0;
    float b = 0.0;
    if(gl_FragCoord.x < koordinate[0])
    {
        r = 1.0;
    }
    if(512.0-gl_FragCoord.y < koordinate[1])
    {
        b = 1.0;
    }
    fColor = vec4(r, 0.0, b, 1.0); //Aufgabe 6
    */
    if(gommemode == 0)
    {
        fColor = vec4(vfColor.rgb, 1.0);
    }
    if(gommemode == 1)
    {
        fColor = vec4(vfColor.gbr, 1.0);
    }
    if(gommemode == 2)
    {
        fColor = vec4(vfColor.brg, 1.0);
    }
    
    //fColor = vfColor;
}