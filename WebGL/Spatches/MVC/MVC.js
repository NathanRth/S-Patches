// Fonctions pour les MVC

function mod(n, m) {
	return ((n % m) + m) % m;
}

// calcule les coordonnées MVC de <point> en fonction du voisinage prise en compte <neighbour_list>
// TO DO : for loop ?
// NOTE : définition simple mais qui ne semble pas permettre de définir des points
// sur les bords du domaine paramétrique
function computeMVC(point,neighbour_list) {
	
	let weights = [];
	let current_weight;

	let coordsSum = 0.0;
	let current_array_index = 0;
	
	let first_neighbour = neighbour_list[mod(current_array_index,neighbour_list.length)];
	let current_neighbour = first_neighbour;
	let prev_neighbour = neighbour_list[mod(current_array_index+1,neighbour_list.length)];
	let next_neighbour = neighbour_list[mod(current_array_index-1,neighbour_list.length)];

	let current_vect,prev_vect,next_vect;

	let current_norm;
	let prev_angle,next_angle;

	do {
		
		current_vect = substraction(current_neighbour,point);
		prev_vect = substraction(prev_neighbour,point);
		next_vect = substraction(next_neighbour,point);		
		
		prev_angle = angle(prev_vect,current_vect);
		next_angle = angle(current_vect, next_vect);

		current_norm = norm(current_vect);


		
		if(current_norm === 0) {
			weights = new Array(neighbour_list.length).fill(0);
			weights[current_array_index] = 1;						
			return weights;
		}

		
		
		current_weight = (Math.tan(prev_angle/2.0)+Math.tan(next_angle/2.0)) / norm(current_vect);	

			

		weights.push(current_weight);
		coordsSum += current_weight;

		current_array_index += 1;
		current_neighbour = neighbour_list[mod(current_array_index,neighbour_list.length)];
		prev_neighbour = neighbour_list[mod(current_array_index+1,neighbour_list.length)];
		next_neighbour = neighbour_list[mod(current_array_index-1,neighbour_list.length)];

	} while(first_neighbour != current_neighbour);
	
	for (let i = 0 ; i < weights.length; i++) {		
		weights[i] /= coordsSum;
	}	
	
	return weights;
}


// définition des MVC permettant de définir des points sur les bords du domaine
// paramétrique
function computeMVC2(point,neighbour_list,polygon_size) {
	let weights = [];
	let current_weight;

	let coordsSum = 0.0;

	let di_1,di1;
	let dj,dj1;

	let ri_1, ri1;
	let rj, rj1;

	let MVC_product;

	for (let i = 0; i < polygon_size; i++) {
		
		let indexi_1 = mod(i-1,polygon_size);
		let indexi = mod(i,polygon_size);
		let indexi1 = mod(i+1,polygon_size);
		MVC_product = 1;

		//v_i-1
		let vi_1 = neighbour_list[indexi_1];
		//v_i+1
		let vi1 = neighbour_list[indexi1];
		
		//d_i = v_i - x
		//d_i-1
		di_1 = substraction(vi_1,point);
		//d_i+1
		di1 = substraction(vi1,point);	

		//r_i = |v_i - x| =  norm(v_i - x) = norm(d_i)
		//r_i-1
		ri_1 = norm(di_1);
		//r_i+1
		ri1 = norm(di1);

		for (let j = 0; j < polygon_size; j++) {
			if(j!==indexi && j!==indexi_1) {

				let indexj = mod(j,polygon_size);
				let indexj1 = mod(j+1,polygon_size);

				let vj = neighbour_list[indexj];
				let vj1 = neighbour_list[indexj1];

				//d_i
				dj = substraction(vj,point);
				//d_i+1
				dj1 = substraction(vj1,point);	
				//r_i
				rj = norm(dj);
				//r_i+1
				rj1 = norm(dj1);
				
				let valuez = Math.sqrt((rj*rj1) + dot(dj,dj1));
				valuez = (valuez>0)?valuez.toFixed(8):0;
				
				MVC_product *= valuez;
				
			}
		}
		
		current_weight = Math.sqrt((ri_1*ri1) - dot(di_1,di1)) * MVC_product;

		weights.push(current_weight);
		coordsSum += current_weight;
	} 
	
	for (let i = 0 ; i < weights.length; i++) {		
		weights[i] /= coordsSum;
	}		
	return weights;	
}

// Calcul de coordonnées 3d en fonction des MVC
function computeCoordsFromMVC(parents,polygon_size,MVCs) {
	
	let coords = Vec3(0,0,0);
	
	for(let i = 0; i < polygon_size; i++) {
		coords = coords.add((parents[i].get3DCoords()).mult(MVCs[i]));
	}
	
	return coords;
}