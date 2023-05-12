"use strict" ;

let gl;
let program;
let positions,
	colors;
let textureLoc

// ToDo 1.7 : Erstelle Variable fuer das Array von
// Texturkoordinaten
let textureCoords

let posVBO,
	colorVBO;

// Todo 1.9 : Erstelle eine Variable fuer das VBO, welches
// die Texturkoordinaten enthaelt
let texVBO

let lastTimestamp;

async function main()
{
	// 1. Get canvas and setup WebGL context
    const canvas = document.getElementById ( "gl-canvas" ) ;
	gl = canvas.getContext ( "webgl2" ) ;

	// 2. Configure viewport
	gl.viewport ( 0, 0, canvas.width, canvas.height ) ;
	gl.clearColor ( 1.0, 1.0, 1.0, 1.0 ) ;

	// 4. Init shader program via additional function and bind it
	program = await initShaders ( gl, "shaders/texture.vert.glsl", "shaders/texture.frag.glsl" ) ;
	gl.useProgram(program);
	initTriangles();

	let textureImage = await loadImage ( "./images/cougar.png" ) ;

	// ToDo 1.1 : Erstelle gl-Textur
  	let texture = gl.createTexture()
	// ToDo 1.2 : Aktiviere Textur 0
	gl.activeTexture(gl.TEXTURE0)
	// ToDo 1.3 : Binde aktuelle Textur
	gl.bindTexture(gl.TEXTURE_2D, texture)
	// ToDo 1.4 : Lade aktuelle Textur mit textureImage
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, textureImage)
	// ToDo 1.5 : Setze Texturparameter ( min, mag )
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR)
	// ToDo 1.6 : Bestimme UniformLocation der Textur ( siehe ToDo 1.6.1 im Fragment Shader )
	// und setze diese
	textureLoc = gl.getUniformLocation(program, "map")
	gl.uniform1i(textureLoc, 0)

	window.requestAnimationFrame ( render ) ;
};


// Diese Funktion wird vorgegeben und laedt Bilddateien als Javascript Images
function loadImage ( path )
{
	return new Promise ( (resolve, reject) =>
	{
	  const img = new Image() ;
	  img.crossOrigin = 'Anonymous' ; // to avoid CORS if used with Canvas
	  img.src = path ;
	  img.onload = () => {
		resolve(img) ;
	  }
	  img.onerror = e => {
		reject(e) ;
	  }
	} )
} ;


function initTriangles() {

	// Specify geometry, colors, texture coordinates

	positions = [ 	-1.0, -1.0,			//	lower left
				 	-1.0,  1.0, 		//	upper left
				   	 1.0, -1.0,			//	lower right

				  	 1.0,  1.0,			// 	upper right
				  	-1.0,  1.0, 		//	upper left
				   	 1.0, -1.0		];	//	lower right

	// ToDo 1.8 : Erstelle Texturkoordiaten fuer jeden Vertex
	textureCoords = [ 0.0, 1.0,
		0.0, 0.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		1.0, 1.0
	]
	
	colors 	= 	[ 	 0, 1, 0, 1, 		//  lower left
			   		 0, 0, 1, 1,		//	upper left
			   		 1, 1, 0, 1,		//  lower right

			   		 1, 0, 0, 1,		//	upper right
			   		 0, 0, 1, 1,		//	upper left
			   		 1, 1, 0, 1		] ;	//	lower right

	initTriangleBuffers();
}

function initTriangleBuffers() {

	// Create VBO for positions and activate it
	posVBO = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, posVBO);

    // Fill VBO with positions
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

	// Create VBO for colors and activate it
	colorVBO = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, colorVBO);

    // Fill VBO with colors
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);

	// ToDo 1.10 : Erstelle ein VBO fuer die Texturkoordiaten und fuelle
	// dies mit Werten analog zu den anderen VBOs
	texVBO = gl.createBuffer()
	gl.bindBuffer(gl.ARRAY_BUFFER, texVBO)

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoords), gl.STATIC_DRAW)

}

function renderTriangles() {

	// 7.1 Link data in VBO to shader variables
	gl.bindBuffer(gl.ARRAY_BUFFER, posVBO);
	const posLoc = gl.getAttribLocation(program, "vPosition");
	gl.enableVertexAttribArray(posLoc);
	gl.vertexAttribPointer(posLoc, 2, gl.FLOAT, false, 0, 0);

    // 7.2 Link data in VBO to shader variables
    gl.bindBuffer(gl.ARRAY_BUFFER, colorVBO);
	const colorLoc = gl.getAttribLocation(program, "vColor");
	gl.enableVertexAttribArray(colorLoc);
	gl.vertexAttribPointer(colorLoc, 4, gl.FLOAT, false, 0, 0);

	// ToDo 1.11 : Binde den VBO fuer die Texturkoordinaten
	// analog zu obigen VBOs
	gl.bindBuffer(gl.ARRAY_BUFFER, texVBO)
	const texPosLoc = gl.getAttribLocation(program, "vTexCoord")
	gl.enableVertexAttribArray(texPosLoc)
	gl.vertexAttribPointer(texPosLoc, 2, gl.FLOAT, false, 0, 0)

	
	// Render
	gl.clear(gl.COLOR_BUFFER_BIT);
	gl.drawArrays(gl.TRIANGLES, 0, 6);
}

function render(timestamp) {
	const elapsed = timestamp - lastTimestamp;

	renderTriangles()

	lastTimestamp = timestamp;
	window.requestAnimationFrame(render);
}

window.onload = async function () {
	main();
};