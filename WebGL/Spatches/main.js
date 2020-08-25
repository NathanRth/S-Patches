"use strict"

let polygon_vert=`#version 300 es
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

in vec3 position_in;

void main()
{
	gl_PointSize = 12.0;
	gl_Position = projectionMatrix * viewMatrix * vec4(position_in,1);
}
`;


let eval_vert=`#version 300 es
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

in vec3 position_in;
in vec2 texcoord_in;

out vec2 texCoord;



void main()
{
	gl_PointSize = 15.0;
	gl_Position = projectionMatrix * viewMatrix * vec4(position_in,1);
	texCoord = 0.55 * (texcoord_in + 0.9);
}
`;

let eval_frag=`#version 300 es
precision highp float;

uniform sampler2D TU0;
uniform vec3 color;
uniform bool texture_on;


in vec2 texCoord;

out vec4 frag_out;

void main()
{
	if(texture_on) {
		vec4 texColor = texture(TU0,texCoord); 
		frag_out = texColor;
	} else {
		frag_out = vec4(color,1);	
	}

}
`;


let color_frag=`#version 300 es
precision mediump float;
out vec4 frag_out;
uniform vec3 color;
void main()
{
	frag_out = vec4(color,1);
}
`;

let prg_polygon = null;
let prg_eval = null;


// ----------------------------------------
// Constantes

let pi = 6.28318530718;
// ----------------------------------------


// ----------------------------------------
// Stockage des points de controle et des segments (_l) et points d eval

let vbo = null;
let vao = null;

let vbo_l = null;
let vao_l = null;

let vbo_eval = null;
let vao_eval = null;

// surface à afficher
let surface = [];

// texture 
let texture = null;

//=========================================================================================

// création d'un spatch
let S_patch = new Spatch(5,2);

// appliquer une texture
let apply_texture = false;


function init_wgl()
{
	//create_interf();
	scene_camera.set_scene_radius(10);

	// ShaderProgram pour affichage géométrique
	prg_polygon = ShaderProgram(polygon_vert,color_frag,'polygon');
	prg_eval = ShaderProgram(eval_vert,eval_frag,'eval');

	// texture à appliquer
	texture = Texture2d();        
	texture.load("textures/orangescr.jpg").then(update_wgl);


	// VBO et VAO pour les points de controle	
	vbo = VBO(S_patch.data_cp,3);	
	vao = VAO([POSITION_ATTRIB,vbo]);

	// VBO et VAO pour les segments
	vbo_l = VBO(S_patch.data_cplines,3);	
	vao_l = VAO([POSITION_ATTRIB,vbo_l]);

	// ordre affichage évaluation
    for(let i = 0; i < S_patch.gl_indices.length; i+=3)
    {

        let i0 = S_patch.gl_indices[i];   // indice premier point du triangle
		let i1 = S_patch.gl_indices[i+1]; // indice second point
        let i2 = S_patch.gl_indices[i+2]; // indice troisième point

        let p0 = S_patch.evaluation_points[i0].get3DCoords(); // 1er point du triangle
        let p1 = S_patch.evaluation_points[i1].get3DCoords(); // 2nd
		let p2 = S_patch.evaluation_points[i2].get3DCoords(); // 3e
		
        surface.push(p0.x);
		surface.push(p0.y);
		surface.push(p0.z);
		surface.push(p1.x);
		surface.push(p1.y);
		surface.push(p1.z);
		surface.push(p2.x);
		surface.push(p2.y);
		surface.push(p2.z);

    }

	// VBO et VAO pour les points d'évaluation
	vbo_eval = VBO(surface,3);	
	vao_eval = VAO([POSITION_ATTRIB,vbo_eval],[TEXCOORD_ATTRIB, vbo_eval]);	




}

function draw_wgl()
{
	gl.enable(gl.DEPTH_TEST);
	gl.clearColor(1,1,1,1);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	const projection_matrix = scene_camera.get_projection_matrix();
	const view_matrix = scene_camera.get_view_matrix();

	// affichage des points de controle
	prg_polygon.bind();
	update_uniform('projectionMatrix', projection_matrix);
	update_uniform('viewMatrix', view_matrix);
	update_uniform('color', 0,0,1);
	vao.bind();
	gl.drawArrays(gl.POINTS,0,S_patch.data_cp.length/3);
	unbind_vao();
	unbind_shader();

	// affichage des segments entre points de controle
	prg_polygon.bind();
	update_uniform('projectionMatrix', projection_matrix);
	update_uniform('viewMatrix', view_matrix);
	update_uniform('color', 1,0,1);
	vao_l.bind();	
	gl.drawArrays(gl.LINES,0,S_patch.data_cplines.length/3);
	unbind_shader();

	// affichage des points d'evaluation
	prg_eval.bind();
	update_uniform('projectionMatrix', projection_matrix);
	update_uniform('viewMatrix', view_matrix);
	update_uniform('texture_on',apply_texture);
	update_uniform('color', 1.0,0.55,0.0);
	texture.bind(0);
	vao_eval.bind();	
	gl.drawArrays(gl.TRIANGLES,0,surface.length/3);
	unbind_shader();
}

launch_3d();

