let gl;
let program;
let meshes = []

// TODO: 1.0:
// Führe globale Variablen ein für Werte, die in verschiedenen Funktionen benötigt werden
let lastTimestamp = 0.0;
let viewMatrix = glMatrix.mat4.create();
let viewMatrixLoc;
let projectionMatrix = glMatrix.mat4.create();
let modelViewProjection = glMatrix.mat4.create();
let modelMatrix;
let kaLoc, kdLoc, ksLoc;
let Ia = [0.3, 0.3, 0.3, 1.0]
let Id = [0.8, 0.8, 0.8, 1.0]
let Is = [0.7, 0.7, 0.7, 1.0]

let target, eye, up;

class Mesh {
	constructor (positions, colors, indices) {
		this.positions = positions;
		this.colors = colors;
		this.indices = indices;
		this.initalized = false;
		this.modelMatrix = glMatrix.mat4.create();
	}
	
	intialize() {
		this.initalized = true;
		
		// Create VBO for positions and activate it
		this.posVBO = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this.posVBO);
		
		// Fill VBO with positions
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.positions), gl.STATIC_DRAW);
		
		// Create VBO for colors and activate it
		this.colorVBO = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this.colorVBO);
		
		// Fill VBO with colors
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.colors), gl.STATIC_DRAW);
		
		// Create VBO for indices and activate it
		this.indexVBO = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexVBO);
		
		// Fill VBO with indices
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint32Array(this.indices), gl.STATIC_DRAW);
	}
	
	update() {
		// TODO 2.6
		// Aktualisiere die Variablen der Vertex und Fragment Shader hier.
		gl.uniformMatrix4fv(modelMatrix, false, this.modelMatrix)
		gl.uniform4fv(kaLoc, this.kvalues[0])
		gl.uniform4fv(kdLoc, this.kvalues[1])
		gl.uniform4fv(ksLoc, this.kvalues[2])
	}

	// TODO 1.3
	// Erweitere die Klasse, so dass diese eine setModelMatrix Funktion bereitstellt.
	setModelMatrix(mat) {
		// TODO
		this.modelMatrix = mat;
	}

	setKvalues(kvalues) {
		this.kvalues = kvalues
	}

	render() {
		if (!this.initalized) {
			this.intialize();
		}

		// Link data in VBO to shader variables
		gl.bindBuffer(gl.ARRAY_BUFFER, this.posVBO);
		const posLoc = gl.getAttribLocation(program, "vPosition");
		gl.enableVertexAttribArray(posLoc);
		// 2. Change number of components per position to 3
		gl.vertexAttribPointer(posLoc, 3, gl.FLOAT, false, 0, 0);

		// Link data in VBO to shader variables
		gl.bindBuffer(gl.ARRAY_BUFFER, this.colorVBO);
		const colorLoc = gl.getAttribLocation(program, "vColor");
		gl.enableVertexAttribArray(colorLoc);
		gl.vertexAttribPointer(colorLoc, 4, gl.FLOAT, false, 0, 0);

		// Bind data in index VBO
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexVBO);

		// 4. Match number of vertices to size of new positions array
		gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_INT, 0);
	}
}

function meshConverter(model) {
	// 1. Get positions from the cube
	let positions = model.meshes[0].vertices;

	// 2. Generate colors for the cube
	let colors;
	if (model.meshes[0].colors) {
		// if the mesh supports colors
		colors = model.meshes[0].colors.flat()
	} else {
		// if not we set one
		colors = model.meshes[0].vertices.map(x => [1, 0.2, 0.5, 1]).flat()
	}

	// 3. Get indices from the cube and flatten them
	let indices = model.meshes[0].faces.flat();
	return new Mesh(positions, colors, indices);
}

// TODO 2.8: Erstelle einen Event-Handler, der anhand von WASD-Tastatureingaben
// die View Matrix anpasst
function move(e) 
{
	switch(e.key.toLowerCase()) {
		case 'w':
			target[2] -= 1.0
			eye[2] -= 1.0
			break
		case 'a':
			target[0] -= 1
			eye[0] -= 1
			break
		case 's':
			target[2] += 1
			eye[2] += 1
			break
		case 'd':
			target[0] += 1
			eye[0] += 1
			break
	}
	glMatrix.mat4.lookAt(viewMatrix, eye, target, up)
	gl.uniformMatrix4fv(viewMatrixLoc, false, viewMatrix)
}
const speed = 0.01
function changeView(e) 
{
	glMatrix.vec3.rotateY(target, target, eye, -e.movementX * speed)
	glMatrix.vec3.rotateX(target, target, eye, -e.movementY * speed)
	glMatrix.mat4.lookAt(viewMatrix, eye, target, up)
	gl.uniformMatrix4fv(viewMatrixLoc, false, viewMatrix)
}

async function main() {

	// Get canvas and setup WebGL context
    const canvas = document.getElementById("gl-canvas");
	gl = canvas.getContext('webgl2');

	// Configure viewport
	gl.viewport(0,0,canvas.width,canvas.height);
	gl.clearColor(1.0,1.0,1.0,1.0);

	// 5. Add depth test
	gl.enable(gl.DEPTH_TEST);

	// Init shader program via additional function and bind it
	program = await initShaders(gl, "shaders/simple.vert.glsl", "shaders/simple.frag.glsl");
	gl.useProgram(program);

	// TODO 2.3: Bestimme Locations der Shadervariablen für Model und View Matrix
	viewMatrixLoc = gl.getUniformLocation(program, "viewMatrix")
	modelMatrix = gl.getUniformLocation(program, "modelMatrix")
	// Bestimme Location fuer k-variablen
	kaLoc = gl.getUniformLocation(program, "ka")
	kdLoc = gl.getUniformLocation(program, "kd")
	ksLoc = gl.getUniformLocation(program, "ks")
	// TODO 2.4: Erstelle mithilfe der Funktionen aus gl-matrix.js eine initiale View Matrix
	eye = glMatrix.vec3.fromValues(0.0, 0.3, 14.0)
	target = glMatrix.vec3.fromValues(0.0, 0.3, 0.0)
	up = glMatrix.vec3.fromValues(0.0, 1.0, 0.0)
	
	glMatrix.mat4.lookAt(viewMatrix, eye, target, up);
	
	// TODO 2.5: Übergebe die initiale View Matrix an den Shader
	gl.uniformMatrix4fv(viewMatrixLoc, false, viewMatrix)
	// TODO 2.9: Füge einen Event Listener für Tastatureingaben hinzu
	document.addEventListener('mousemove', changeView)
	document.addEventListener('keypress', move)

	let files = [
		// TODO 1.1
		// Add your trees and clouds here
		// Load the island as ply object
		"meshes/island.ply",
		"meshes/baum.ply",
		"meshes/wolke.ply",
		"meshes/baum.ply",
		"meshes/wolke.ply",
	];

	let matrices = [
		// TODO 1.2
		// create model matrices for each object
		glMatrix.mat4.create(),
		glMatrix.mat4.create(),
		glMatrix.mat4.create(),
		glMatrix.mat4.create(),
		glMatrix.mat4.create(),
	];
	// Indexreihenfolge: 0=ka, 1=kd, 2= ks
	kinsel = [[0.4, 0.2, 0.0, 1.0],
			  [0.6, 0.3, 0.0, 1.0], 
			  [0.7, 0.7, 0.7, 1.0]]

	kbaum =  [[0.2, 0.5, 0.0, 1,0],
			  [0.4, 0.8, 0.2, 1.0],
			  [0.6, 0.9, 0.2, 1.0]]
			  
	kwolke = [[0.9, 0.9, 0.9, 1],
			  [0.9, 0.9, 0.9, 1], 
			  [1.0, 1.0, 1.0, 1.0]]

	let kvalues = [
		kinsel,
		kbaum,
		kwolke,
		kbaum,
		kwolke
	]

	let baum2 = matrices[2];
	glMatrix.mat4.translate(baum2, baum2, [1, 0, 1])
	glMatrix.mat4.scale(baum2, baum2, [0.5, 0.5, 0.5])
	let wolke2 = matrices[3];
	glMatrix.mat4.translate(wolke2, wolke2, [2, 0, 2])

	for (let i = 0; i < files.length; i++) {
		let mesh = await readMeshAsync(files[i], meshConverter);

		// TODO 1.3 Implementiere die Funktion setModelMatrix,
		// welche die Model Matrix des jeweiligen Meshes setzt. 
		mesh.setModelMatrix(matrices[i]);
		mesh.setKvalues(kvalues[i])
		meshes.push(mesh);
	}

	window.requestAnimationFrame(render);
};

function render(timestamp) {
	const elapsed = timestamp - lastTimestamp;

	// TODO 1.4
	// Clear frame here
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	// TODO 1.5
	// Render objects here
	for (let i = 0; i < meshes.length; ++i) {
		// TODO 2.7
		// Call mesh update function here
		meshes[i].update()
		meshes[i].render();
	}
	// ...

	lastTimestamp = timestamp;
	window.requestAnimationFrame(render);
}

window.onload = async function () {
	main();
};

