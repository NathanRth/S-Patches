// Ajoute un point de controle de base (points présent en degré 1)
function add_control_point(polygon_size, position) {

	let point_index = new Array(polygon_size).fill(0);
	point_index[position] = 1;
	let coords = Vec3( Math.cos(pi/8+pi*(position/polygon_size)) , Math.sin(pi/8+pi*(position/polygon_size)),0);
	
	return new ControlPoint(coords,point_index,1);
}

// Calcule toutes les distributions possibles du degré parmi un vecteur <array> 
// initialisé à 0 et de taille égale au nombre de cotés du polygone
// exemple pentagone degré 2 :[20000],[02000],[11000],[00200], etc...
function generate_distribution(distributions, array, degree, current_id) {
    if (current_id == array.length - 1) {
        array[current_id] = degree;
        distributions.push(array);
    }
    else {
        for (let i = 0; i <= degree; ++i) {
            let array_copy = array.slice();
            array_copy[current_id] = i;
            generate_distribution(distributions, array_copy, degree - i, current_id + 1);
        }
	}	
    return distributions;
}

function getControlPointFromList(index,points) {
	let point = null;

	points.forEach(function(d) {		
		if(index.join() === d.getIndex().join()) {
			point = d;
		}
	});

	return point;	
}
