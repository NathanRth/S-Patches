
// classe s-patch
// polygon_size : (int) nombre de cotés du polygone
// degree : int
// control_points : liste des points de contrôle du spatch (comporte points présents dégre 1 à n)
// (exemple : si on a élever le degré jusqu'à 5, on a les points de degré 1,2,3,4,5)
// lines : liste des points représentant les liens entre les points de contrôle, sont groupés
//         par deux pour les passer au shader
// parametric_domain : domaine paramétrique du S-patch
// fcontrol_points : points de controle de degré n uniquement
// sampling parameter : finesse d'échantillonage
function Spatch (polygon_size,degree,sampling_parameter,is_restricted) {

    this.polygon_size = polygon_size || 5;
	this.degree = degree || 2;
	this.sampling_parameter = sampling_parameter || 15;
	
    this.control_points = [];
	this.parametric_domain = [];
	this.fcontrol_points = [];

	// données pour les VBO
	this.data_cp = [];
	this.data_cplines = [];
	this.data_evaluation = [];

	// Calcul points de contrôle
	this.computeControlPoints();
	this.computeFinalControlPoints();
	this.computeConnexity();


	// Restriction du domaine pour avoir un trou
	this.domain_is_restricted = (is_restricted && (this.polygon_size === 4)) || false;

	// Pré-conditions : zone doit être rectangulaire, valeurs entre -sqrt(2)/2 et sqrt(2)/2
	//  et carreau 4 cotés
	this.restricted_domain_xmin = -0.4;
	this.restricted_domain_ymin = -0.4;
	this.restricted_domain_xmax = 0.4;
	this.restricted_domain_ymax = 0.4;

	//domaine restreint
	this.restriction_domain = [Vec2(this.restricted_domain_xmin,this.restricted_domain_ymin),
							   Vec2(this.restricted_domain_xmax,this.restricted_domain_ymin),
							   Vec2(this.restricted_domain_xmax,this.restricted_domain_ymax),
							   Vec2(this.restricted_domain_xmin,this.restricted_domain_ymax)];

	// Echantillonnage et évaluation
	this.gl_indices = [];
	this.sampled_points = [];
	this.parametric_sampling();
	this.evaluation_points = [];
	this.evaluation();
}

Spatch.prototype.getPolygonSize = function() {
    return this.polygon_size;
}

Spatch.prototype.setPolygonSize = function(polygon_size) {
    this.polygon_size = polygon_size;
}



Spatch.prototype.getDegree = function() {
    return this.degree;
}

Spatch.prototype.setDegree = function(degree) {
    this.degree = degree;
}



Spatch.prototype.getControlPoints = function() {
    return this.control_points;
}

Spatch.prototype.setControlPoints = function(control_points) {
    this.control_points = control_points;
}


Spatch.prototype.computeControlPoints = function() {

	// base control points (= degree 1)
	for (let i = 0; i < this.polygon_size; i++) {
        let current_point = add_control_point(this.polygon_size,i);
		this.control_points.push(current_point);				
        this.parametric_domain.push(Vec2(current_point.get3DCoords().x,current_point.get3DCoords().y));
	}
	
	// création des points de controle de degrés supérieurs à 1 (élévation)
	this.computeHigherDegreeCoords();

}

// créée les points de controle jusqu'au degré indiqué <degree> (de 2 à <degree>)
Spatch.prototype.computeHigherDegreeCoords = function() {
    let that = this;
    for (let i = 2; i <= this.degree; i++) {
		let distributions = [];
		let combinaisons = generate_distribution(distributions,new Array(this.polygon_size).fill(0),i,0);

		combinaisons.forEach(function(d) {            
			that.control_points.push(new ControlPoint(that.computeNewCoords(d),d,i));
		});		
	}
}

// calcul des coordonnées 3D du vecteur d'indice <control_point_index> d'un point de controle
Spatch.prototype.computeNewCoords = function(control_point_index) {
    let that = this;
	let new_coords = [0,0,0];
	control_point_index.forEach(function(d,i) {

		if(d>0) {
			let index_deg_min_1 = control_point_index.slice();
			index_deg_min_1[i] = index_deg_min_1[i] - 1; 
			 
			let point_deg_min_1 = that.getControlPointByIndex(index_deg_min_1);
			let point_weight = d/that.degree;
						
			new_coords[0] += point_weight * point_deg_min_1.get3DCoords().x;
			new_coords[1] += point_weight * point_deg_min_1.get3DCoords().y;
			new_coords[2] += point_weight * point_deg_min_1.get3DCoords().z;

		}
	});	
	new_coords[2] = Math.pow(new_coords[0],2) + Math.pow(new_coords[1],2);
	return Vec3(new_coords[0],new_coords[1],new_coords[2]);
} 




// Recupere le point ayant pour vecteur d'index <index> dans la liste de points de controle
Spatch.prototype.getControlPointByIndex = function(index) {
	let point = null;

	this.control_points.forEach(function(d) {		
		if(index.join() === d.getIndex().join()) {
			point = d;
		}
	});

	return point;	
}

// points par degré
Spatch.prototype.getControlPointsByDegree = function(degree) {
	let points = [];
	this.control_points.forEach(function(d) {		
		if(vectorSum(d.getIndex()) === degree) {
			points.push(d);
		}
	});
	return points;
}

// boite englobante (pas utilisé)
Spatch.prototype.getBoundingBoxDiagonalLength = function() {
	let points = this.getControlPointsByDegree(this.degree);
	
	let min_x = points[0].get3DCoords().x;
	let min_y = points[0].get3DCoords().y;
	let min_z = points[0].get3DCoords().z;
	let max_x = points[0].get3DCoords().x;
	let max_y = points[0].get3DCoords().y;
	let max_z = points[0].get3DCoords().z;

	points.forEach(function (point) {

		if (point.get3DCoords().x < min_x) {
			min_x = point.get3DCoords().x;
		}
		if (point.get3DCoords().x > max_x) {
			max_x = point.get3DCoords().x;
		}
		if (point.get3DCoords().y < min_y) {
			min_y = point.get3DCoords().y;
		}
		if (point.get3DCoords().y > max_y) {
			max_y = point.get3DCoords().y;
		}
		if (point.get3DCoords().z < min_z) {
			min_z = point.get3DCoords().z;
		}
		if (point.get3DCoords().z > max_z) {
			max_z = point.get3DCoords().z;
		}
	});

	//return Math.max(max_x - min_x,max_y - min_y);
	return Math.sqrt(Math.pow(max_x - min_x, 2) + Math.pow(max_y - min_y, 2) + Math.pow(max_z - min_z, 2));
}

// points de degré n désiré uniquement
Spatch.prototype.computeFinalControlPoints = function() {
	let that = this;
	let points = this.getControlPointsByDegree(this.degree);

	// déplacement de points
	// points[0].get3DCoords().z +=2.5;
	// points[4].get3DCoords().y +=7.5;
	// points[5].get3DCoords().x -=1;
	// points[9].get3DCoords().z +=2.5;
	// points[4].get3DCoords().z +=2.5;

	points.forEach(function(d) {
		that.data_cp.push(d.get3DCoords().x);
		that.data_cp.push(d.get3DCoords().y);
		that.data_cp.push(d.get3DCoords().z);
	});
	that.fcontrol_points = points;
}


// Calcul des aretes selon les regles de connexité/liaisons des points de controle du document sur les s-patches
// NOTE : il existe une différence de 1 au niveau d'un des indices par rapport au document des spatches,
// peut etre s'agit il d'une erreur du document, ou de ma part.
Spatch.prototype.computeConnexity = function() {
	let connex_pairs = [];
    let that = this;
	that.fcontrol_points.forEach(function(fl) {
		that.fcontrol_points.forEach(function(fh) {
			let index_exists = false;
			for (let j = 0; j < that.polygon_size; j++) {
											
					let cond1 = true;
					let j_mod = j%that.polygon_size;
					let j1_mod = (j+1)%that.polygon_size;

					for (let i = 0; i < that.polygon_size; i++) {
						if(fl.getIndex()[i] !== fh.getIndex()[i] && (i!==j_mod) && (i!==j1_mod)) {
								cond1 = false;
						}
					}

					let cond2 = false;
					if ((fl.getIndex()[j1_mod] - 1) === fh.getIndex()[j1_mod]) {
						cond2 = true;
					}					
					
					let cond3 = false;
					if (fl.getIndex()[j_mod] + 1 === fh.getIndex()[j_mod]) {
						cond3 = true;
					}
							
					if(cond1 && cond2 && cond3) {
						index_exists = true;
					}

			}

			if(index_exists) {
					that.data_cplines.push(fl.get3DCoords().x);
					that.data_cplines.push(fl.get3DCoords().y);
					that.data_cplines.push(fl.get3DCoords().z);

					that.data_cplines.push(fh.get3DCoords().x);
					that.data_cplines.push(fh.get3DCoords().y);
					that.data_cplines.push(fh.get3DCoords().z);						
			}
		});
	});
	return that.data_cplines;	
}


// évaluation d'un point du domaine (position 2D), conversion mvc et eval
Spatch.prototype.evaluatePoint = function(eval_point) {
    
    let that = this;
	let points = that.control_points.slice();	

	let MVCs = computeMVC2(eval_point,that.parametric_domain,this.polygon_size);
		
	for(let i = that.degree - 1; i >= 0; i--) {

		let distributions = [];
		let current_cps = generate_distribution(distributions,new Array(that.polygon_size).fill(0),i,0);		
 
		current_cps.forEach(function(d,j,arr) {
			let parents = getParentCPs(d,points,that.polygon_size);			
			let new_3D_coords = computeCoordsFromMVC(parents,that.polygon_size,MVCs);			
			arr[j] = new ControlPoint(new_3D_coords,d,i);
		});

		points = current_cps;		
	}
	return points[0];
}


// Si le degré du Patch est égal à n, calculer le polygone de degré n-1 avec De Casteljau,
// jusqu'à arriver à un point unique (= le point d'évaluation du SPatch pour un point <eval_point> du domaine
// paramétrique donné)
Spatch.prototype.evaluation = function() {
    
    let that = this;
	let points = that.fcontrol_points.slice();	

	this.sampled_points.forEach(function(d) {
		let ev_point = that.evaluatePoint(d);
		that.evaluation_points.push(ev_point);
		
		that.data_evaluation.push(ev_point.get3DCoords().x);
		that.data_evaluation.push(ev_point.get3DCoords().y);
		that.data_evaluation.push(ev_point.get3DCoords().z);
	});
}

// [1,0,0,0,0] => [2,0,0,0,0],[1,1,0,0,0],[1,0,1,0,0],[1,0,0,1,0],[1,0,0,0,1] 
// ajout de 1 à chaque indice, pour identifier de quels points est dépedant celui passé
// en paramètre
function getParentCPs(index_point,point_list,polygon_size) {
	let parentCPs = [];
	for(let ind_iter = 0; ind_iter < polygon_size ; ind_iter++) {
		let current_index = (index_point.slice());
		current_index[ind_iter]++;
		parentCPs.push(getControlPointFromList(current_index,point_list));
	}
	return parentCPs;
}


// échantillonage polygonal pour affichage nuage de points (pas triangulé)
// Spatch.prototype.parametric_sampling = function(t) {
// 	let parametric_domain = this.parametric_domain.slice();
// 	let polygon_size = this.polygon_size;

// 	let distributions = [];
// 	generate_distribution(distributions,new Array(polygon_size).fill(0),t,0);

// 	distributions.forEach(function(d,i,arr) {
// 		arr[i].forEach(function(d2,i2,arr2) {
// 			arr2[i2] /= t;
// 		});

// 		arr[i] = computeSampled2DCoords(arr[i],parametric_domain);
		
// 	});
	
// 	return distributions;
	
// }


// Echantillonage par triangulation
Spatch.prototype.parametric_sampling = function() {
	let O = Vec2(0,0);
	if(this.domain_is_restricted) {
		
		for (let i = 0; i < this.polygon_size; i++)
		{
			this.sampleTriangle(this.restriction_domain[i], this.parametric_domain[mod(i + 2, this.polygon_size)], this.parametric_domain[mod(i + 3, this.polygon_size)]);
			this.genIndicesList();
		}

		for (let i = 0; i < this.polygon_size; i++)
		{
			this.sampleTriangle(this.restriction_domain[i], this.restriction_domain[mod(i + 1, this.polygon_size)], this.parametric_domain[mod(i + 3, this.polygon_size)]);
			this.genIndicesList();
		}
	} else {
		for (let i = 0; i < this.polygon_size; i++)
		{
			this.sampleTriangle(O, this.parametric_domain[i], this.parametric_domain[mod(i + 1, this.polygon_size)]);
			this.genIndicesList();
		}
	}



}


// Echantillonage triangle OAB
Spatch.prototype.sampleTriangle = function(O,A,B) {

	for (let i = 0; i <= this.sampling_parameter; i++)
    {
		let multiplier = i/this.sampling_parameter;
		let sp = A.add(substraction(B,A).mult(multiplier));		
        this.sampled_points.push(sp);
	}
	
	for (let i = 1; i <= this.sampling_parameter; i++)
    {
		let multiplier = i/this.sampling_parameter;
		let rs = A.add(substraction(O,A).mult(multiplier));
		let re = B.add(substraction(O,B).mult(multiplier));		

        if (this.sampling_parameter - i != 0)
        {
            for (let j = 0; j <= this.sampling_parameter - i; j++)
            {
				let multiplier2 = j/(this.sampling_parameter-i);
				let sp = rs.add(substraction(re,rs).mult(multiplier2));
				this.sampled_points.push(sp);
            }

        }
        else
        {
			this.sampled_points.push(O);
        }
	}
	
}

// génération des indices des triangles
Spatch.prototype.genIndicesList = function()
{
	let n = 0;
    if (this.gl_indices.length > 0 )
    {
		n =	Math.max.apply(Math, this.gl_indices) + 1;
	}
		
    let l = this.sampling_parameter;

    for (let i = 0; i < this.sampling_parameter; i++)
    {
        l = this.sampling_parameter - i;
        let nbtri = this.sampling_parameter * 2 - 1 - i * 2;
		let k = 0;
		
        for (let j = 0; j < nbtri; j++)
        {			

				if (mod(j,2) == 0)
				{
					this.gl_indices.push(n + k);
					this.gl_indices.push(n + l + 1 + k);
					this.gl_indices.push(n + 1 + k);						
				}
				else  {
					k++;
					this.gl_indices.push(n + k);
					this.gl_indices.push(n + l + k);
					this.gl_indices.push(n + l + 1 + k);
				}

        }
        n = n + l + 1;
	}
	
}

// fonction plus utilisée
// test si trois points d'un triangle appartiennent à notre domaine (et ne sont donc
// pas dans la partie restreinte du domaine)
Spatch.prototype.isTriangleInRestrictedDomain = function(trianglePointsArray) {

	if(this.domain_is_restricted === false) {
		return true;
	}

	let that = this;
	let result = true;


	trianglePointsArray.forEach(function(d) {				
		if (d.x > that.restricted_domain_xmin && d.x < that.restricted_domain_xmax && d.y > that.restricted_domain_ymin && d.y < that.restricted_domain_ymax) {
			result = false;
		}
	});

	if(result == false) {
		console.log(trianglePointsArray);
	}
	

	return result;
}
 


// plus utilisée
function computeSampled2DCoords(weights,parametric_domain) {
	let coords2D = [0,0];
	for (let i = 0; i < weights.length; i++) {
		coords2D[0] += weights[i] * parametric_domain[i].x;
		coords2D[1] += weights[i] * parametric_domain[i].y;		
	}
	
	return Vec2(coords2D[0],coords2D[1]);
	
}





