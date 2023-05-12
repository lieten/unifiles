let gl;
let program;
let positions,
	colors;
let posVBO,
	colorVBO;
let lastTimestamp;
let koordinate = [100.0, 100.0]
let koordinateLoc
let gommemode = 0
let gommemodeLoc

async function main() {

	// 1. Get canvas and setup WebGL context
    const canvas = document.getElementById("gl-canvas");
	gl = canvas.getContext('webgl2');

	// 2. Configure viewport
	gl.viewport(0,0,canvas.width,canvas.height);
	gl.clearColor(1.0,1.0,1.0,1.0);

	// 4. Init shader program via additional function and bind it
	program = await initShaders(gl, "shaders/simple.vert.glsl", "shaders/simple.frag.glsl");
	gl.useProgram(program);

	koordinateLoc = gl.getUniformLocation(program, "koordinate")
	gommemodeLoc = gl.getUniformLocation(program, "gommemode")

	initTriangle();
	window.requestAnimationFrame(render);
};

function initTriangle() {

	// 3. Specify geometry
	positions = [ -1.0, -1.0, 
				  -1.0,  1.0, 
				   1.0, -1.0,
				   1.0,  1.0,
				  -1.0,  1.0, 
				   1.0, -1.0];
	colors = [ 0, 1, 0, 1, 
			   0, 0, 1, 1,
			   1, 1, 0, 1,
			   1, 0, 0, 1,
			   0, 0, 1, 1,
			   1, 1, 0, 1];

	initTriangleBuffers();
}

function initTriangleBuffers() {

	// 5.1 Create VBO for positions and activate it
	posVBO = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, posVBO);

    // 6.1 Fill VBO with positions
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

	// 5.2 Create VBO for colors and activate it
	colorVBO = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, colorVBO);

    // 6.2 Fill VBO with colors
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
}

function renderTriangle() {

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

	// 8. Render
	gl.clear(gl.COLOR_BUFFER_BIT);
	gl.drawArrays(gl.TRIANGLES, 0, 6);
}

function render(timestamp) {
	const elapsed = timestamp - lastTimestamp;

	renderTriangle()
	//Setze Koordinate für Schnittstelle
	//koordinate = [Math.random()* 512, Math.random()* 512] //epilepsy warning
	gl.uniform2fv(koordinateLoc, koordinate)
	gl.uniform1i(gommemodeLoc, gommemode)
	//gommemode = (gommemode + 0.1) % 3
	lastTimestamp = timestamp;
	window.requestAnimationFrame(render);
}

window.onload = async function () {
	main();
};

